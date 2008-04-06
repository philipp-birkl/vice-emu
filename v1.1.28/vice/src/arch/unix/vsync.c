/*
 * vsync.c - End-of-frame handling for Unix.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Teemu Rantanen <tvr@cs.hut.fi>
 *  Andreas Boose <boose@linux.rz.fh-hannover.de>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

/* This does what has to be done at the end of each screen frame (50 times per
   second on PAL machines). */

#include "vice.h"

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include "clkguard.h"
#include "cmdline.h"
#include "interrupt.h"
#include "kbdbuf.h"
#include "log.h"
#include "maincpu.h"
#include "resources.h"
#include "sound.h"
#include "types.h"
#include "ui.h"
#include "video.h"
#include "vsync.h"

#ifdef HAS_JOYSTICK
#include "joystick.h"
#endif

#ifndef SA_RESTART
#define SA_RESTART 0
#endif

/* ------------------------------------------------------------------------- */

/* Relative speed of the emulation (%).  0 means "don't limit speed".  */
static int relative_speed;

/* Refresh rate.  0 means "auto".  */
static int refresh_rate;

/* "Warp mode".  If nonzero, attempt to run as fast as possible.  */
static int warp_mode_enabled;

/* FIXME: This should call `set_timers'.  */
static int set_relative_speed(resource_value_t v)
{
    relative_speed = (int) v;
    return 0;
}

static int set_refresh_rate(resource_value_t v)
{
    if ((int) v < 0)
        return -1;
    refresh_rate = (int) v;
    return 0;
}

static int set_warp_mode(resource_value_t v)
{
    warp_mode_enabled = (int) v;
    sound_set_warp_mode(warp_mode_enabled);
    return 0;
}

/* Vsync-related resources.  */
static resource_t resources[] = {
    { "Speed", RES_INTEGER, (resource_value_t) 100,
      (resource_value_t *) &relative_speed, set_relative_speed },
    { "RefreshRate", RES_INTEGER, (resource_value_t) 0,
      (resource_value_t *) &refresh_rate, set_refresh_rate },
    { "WarpMode", RES_INTEGER, (resource_value_t) 0,
      (resource_value_t *) &warp_mode_enabled, set_warp_mode },
    { NULL }
};

int vsync_init_resources(void)
{
    return resources_register(resources);
}

/* ------------------------------------------------------------------------- */

/* Vsync-related command-line options.  */
static cmdline_option_t cmdline_options[] = {
    { "-speed", SET_RESOURCE, 1, NULL, NULL, "Speed", NULL,
      "<percent>", "Limit emulation speed to specified value" },
    { "-refresh", SET_RESOURCE, 1, NULL, NULL, "RefreshRate", NULL,
      "<value>", "Update every <value> frames (`0' for automatic)" },
    { "-warp", SET_RESOURCE, 0, NULL, NULL, "WarpMode", (resource_value_t) 1,
      NULL, "Enable warp mode" },
    { "+warp", SET_RESOURCE, 0, NULL, NULL, "WarpMode", (resource_value_t) 0,
      NULL, "Disable warp mode" },
    { NULL }
};

int vsync_init_cmdline_options(void)
{
    return cmdline_register_options(cmdline_options);
}

/* ------------------------------------------------------------------------- */

/* Maximum number of frames we can skip consecutively when adjusting the
   refresh rate dynamically.  */
#define MAX_SKIPPED_FRAMES	10

/* Number of frames per second on the real machine.  */
static double refresh_frequency;

/* Number of clock cycles per seconds on the real machine.  */
static long cycles_per_sec;

/* Function to call at the end of every screen frame.  */
static void (*vsync_hook)(void);

/* ------------------------------------------------------------------------- */

static volatile int elapsed_frames = 0;
static int timer_disabled = 1;
static int timer_speed = 0;
static int timer_patch = 0;

static int timer_ticks;
static struct timeval timer_time;

static int speed_eval_suspended = 1;
static CLOCK speed_eval_prev_clk;

static void update_elapsed_frames(int want)
{
    struct timeval now;
    static int pending;

    if (timer_disabled)
        return;
    if (!want && timer_patch > 0) {
	timer_patch--;
	elapsed_frames++;
    }
    gettimeofday(&now, NULL);
    while (now.tv_sec > timer_time.tv_sec ||
	   (now.tv_sec == timer_time.tv_sec &&
	    now.tv_usec > timer_time.tv_usec)) {
	if (pending)
	    pending--;
	else {
	    if (timer_patch < 0)
		timer_patch++;
	    else
		elapsed_frames++;
	}
	timer_time.tv_usec += timer_ticks;
	while (timer_time.tv_usec >= 1000000) {
	    timer_time.tv_usec -= 1000000;
	    timer_time.tv_sec++;
	}
    }
    if (want == 1 && !pending) {
	if (timer_patch < 0)
	    timer_patch++;
	else
	    elapsed_frames++;
	pending++;
    }
}

static int set_timer_speed(int speed)
{
    if (speed > 0) {
	gettimeofday(&timer_time, NULL);
	timer_ticks = ((100.0 / refresh_frequency) * 1000000) / speed;
	update_elapsed_frames(-1);
	elapsed_frames = 0;
    } else {
	speed = 0;
	timer_ticks = 0;
    }

    timer_speed = speed;
    timer_disabled = speed ? 0 : 1;

    return 0;
}

static void timer_sleep(void)
{
    int	old;

    if (!timer_disabled) {
        old = elapsed_frames;
	while (old == elapsed_frames) {
	    update_elapsed_frames(1);
	    if (old == elapsed_frames)
		usleep(1);
	}
    }
}

static void patch_timer(int patch)
{
    timer_patch += patch;
}

static void display_speed(int num_frames)
{
#ifdef HAVE_GETTIMEOFDAY
    static double prev_time;
    struct timeval tv;
    double curr_time;

    gettimeofday(&tv, NULL);
    curr_time = (double)tv.tv_sec + ((double)tv.tv_usec) / 1000000.0;
    if (!speed_eval_suspended) {
	CLOCK diff_clk;
	double speed_index;
	double frame_rate;

	diff_clk = clk - speed_eval_prev_clk;
	frame_rate = (double)num_frames / (curr_time - prev_time);
	speed_index = ((((double)diff_clk / (curr_time - prev_time))
			/ (double)cycles_per_sec)) * 100.0;
	ui_display_speed((float)speed_index, (float)frame_rate,
                         warp_mode_enabled);
    }

    prev_time = curr_time;
    speed_eval_prev_clk = clk;
    speed_eval_suspended = 0;
#else  /* HAVE_GETTIMEOFDAY */
    /* Sorry, no speed evaluation.  */
    return;
#endif /* HAVE_GETTIMEOFDAY */
}

static void clk_overflow_callback(CLOCK amount, void *data)
{
    speed_eval_prev_clk -= amount;
}

/* ------------------------------------------------------------------------- */

void vsync_set_machine_parameter(double refresh_rate, long cycles)
{
    refresh_frequency = refresh_rate;
    cycles_per_sec = cycles;
}

void vsync_init(void (*hook)(void))
{
    vsync_hook = hook;
    vsync_disable_timer();
    suspend_speed_eval();
    clk_guard_add_callback(&maincpu_clk_guard, clk_overflow_callback, NULL);
}

int vsync_disable_timer(void)
{
    if (!timer_disabled)
	return set_timer_speed(0);
    else
	return 0;
}

/* This should be called whenever something that has nothing to do with the
   emulation happens, so that we don't display bogus speed values. */
void suspend_speed_eval(void)
{
    sound_suspend();
    speed_eval_suspended = 1;
}

/* This is called at the end of each screen frame.  It flushes the audio buffer
   and keeps control of the emulation speed.  */
int do_vsync(int been_skipped)
{
    static unsigned short frame_counter = USHRT_MAX;
    static unsigned short num_skipped_frames;
    static int skip_counter;
    int skip_next_frame = 0;

    vsync_hook();

    if (been_skipped)
	num_skipped_frames++;

    if (timer_speed != relative_speed) {
	frame_counter = USHRT_MAX;
	if (set_timer_speed(relative_speed) < 0) {
	    log_error(LOG_DEFAULT, "Trouble setting timers... giving up.");
            /* FIXME: Hm, maybe we should be smarter.  But this is should
               never happen.  */
	    exit(-1);
	}
    }

    ui_dispatch_events();

    if (warp_mode_enabled) {
        /* "Warp Mode".  Just skip as many frames as possible and do not
           limit the maximum speed at all.  */
        if (skip_counter < MAX_SKIPPED_FRAMES) {
            skip_next_frame = 1;
            skip_counter++;
        } else {
            skip_counter = elapsed_frames = 0;
        }
        sound_flush(0);
    } else if (refresh_rate != 0) {
	/* Fixed refresh rate.  */
	update_elapsed_frames(0);
	if (timer_speed && skip_counter >= elapsed_frames)
	    timer_sleep();
	if (skip_counter < refresh_rate - 1) {
	    skip_next_frame = 1;
	    skip_counter++;
	} else {
	    skip_counter = elapsed_frames = 0;
	}
        patch_timer(sound_flush(relative_speed));
    } else {
	/* Dynamically adjusted refresh rate.  */
	update_elapsed_frames(0);
	if (skip_counter >= elapsed_frames) {
	    elapsed_frames = -1;
	    timer_sleep();
	    skip_counter = 0;
	} else {
	    if (skip_counter < MAX_SKIPPED_FRAMES) {
		skip_next_frame = 1;
		skip_counter++;
	    } else {
		skip_counter = elapsed_frames = 0;
	    }
	}
        patch_timer(sound_flush(relative_speed));
    }

    if (frame_counter >= refresh_frequency * 2) {
	display_speed(frame_counter + 1 - num_skipped_frames);
	num_skipped_frames = 0;
	frame_counter = 0;
    } else
	frame_counter++;

    kbd_buf_flush();

#ifdef HAS_JOYSTICK
    joystick();
#endif

    return skip_next_frame;
}
