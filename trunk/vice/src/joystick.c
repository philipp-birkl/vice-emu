/*
 * joystick.c - Common joystick emulation.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *
 * Based on old code by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Jouko Valta <jopi@stekt.oulu.fi>
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
 *  Bernhard Kuhn <kuhn@eikon.e-technik.tu-muenchen.de>
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

#include "vice.h"

#include <stdlib.h>
#include <string.h>

#include "alarm.h"
#include "event.h"
#include "keyboard.h"
#include "joy.h"
#include "joystick.h"
#include "maincpu.h"
#include "network.h"
#include "snapshot.h"
#include "ui.h"
#include "types.h"
#include "log.h"

#define JOYSTICK_RAND() (rand() & 0x3fff)

#define JOYSTICK_NUM 3

/* Global joystick value.  */
BYTE joystick_value[JOYSTICK_NUM] = { 0, 0, 0 };

/* Latched joystick status.  */
static BYTE latch_joystick_value[JOYSTICK_NUM] = { 0, 0, 0 };
static BYTE network_joystick_value[JOYSTICK_NUM] = { 0, 0, 0 };

/* to prevent illegal direction combinations */
static const BYTE joystick_opposite_direction[] = 
    { 0, 2, 1, 3, 8, 10, 9, 11, 4, 6, 5, 7, 12, 14, 13, 15 };

static alarm_t *joystick_alarm = NULL;

static CLOCK joystick_delay;

static void joystick_latch_matrix(CLOCK offset)
{
#ifdef HAVE_NETWORK
    if (network_connected())
        memcpy(joystick_value, network_joystick_value, sizeof(joystick_value));
    else
#endif
        memcpy(joystick_value, latch_joystick_value, sizeof(joystick_value));
    ui_display_joyport(joystick_value);
}

/*-----------------------------------------------------------------------*/

static void joystick_event_record(void)
{
    event_record(EVENT_JOYSTICK_VALUE, (void *)joystick_value,
                 sizeof(joystick_value));
}

void joystick_event_playback(CLOCK offset, void *data)
{
    memcpy(latch_joystick_value, data, sizeof(joystick_value));

    joystick_latch_matrix(offset);
}

static void joystick_latch_handler(CLOCK offset, void *data)
{
    alarm_unset(joystick_alarm);
    alarm_context_update_next_pending(joystick_alarm->context);

    joystick_latch_matrix(offset);

#ifdef HAVE_NETWORK
    if (!network_connected())
#endif
        joystick_event_record(); 
}

void joystick_event_delayed_playback(void *data)
{
    memcpy(network_joystick_value, data, sizeof(latch_joystick_value));
    alarm_set(joystick_alarm, maincpu_clk + joystick_delay);
}

void joystick_register_delay(unsigned int delay)
{
    joystick_delay = delay;
}
/*-----------------------------------------------------------------------*/
static void joystick_process_latch(void)
{
#ifdef HAVE_NETWORK
    if (network_connected()) {
        CLOCK joystick_delay = JOYSTICK_RAND();
        network_event_record(EVENT_JOYSTICK_DELAY,
                (void *)&joystick_delay, sizeof(joystick_delay));
        network_event_record(EVENT_JOYSTICK_VALUE, 
                (void *)latch_joystick_value, sizeof(latch_joystick_value));
    } 
    else
#endif
    {
        alarm_set(joystick_alarm, maincpu_clk + JOYSTICK_RAND());
    }
}

void joystick_set_value_absolute(unsigned int joyport, BYTE value)
{
    if (event_playback_active())
        return;

    latch_joystick_value[joyport] = value;
    joystick_process_latch();
}

BYTE joystick_get_value_absolute(unsigned int joyport)
{
    return latch_joystick_value[joyport];
}

void joystick_set_value_or(unsigned int joyport, BYTE value)
{
    if (event_playback_active())
        return;

    latch_joystick_value[joyport] |= value;
    latch_joystick_value[joyport] &= ~joystick_opposite_direction[value & 0xf];
    joystick_process_latch();
}

void joystick_set_value_and(unsigned int joyport, BYTE value)
{
    if (event_playback_active())
        return;

    latch_joystick_value[joyport] &= value;
    joystick_process_latch();
}

void joystick_clear(unsigned int joyport)
{
    latch_joystick_value[joyport] = 0;
    joystick_latch_matrix(0);
}

void joystick_clear_all(void)
{
    memset(latch_joystick_value, 0, JOYSTICK_NUM);
    joystick_latch_matrix(0);
}

/*-----------------------------------------------------------------------*/

#ifdef COMMON_KBD
static int joypad_bits[10] = {
    0x10, 0x06, 0x02, 0x0a, 0x04, 0x00, 0x08, 0x05, 0x01, 0x09
};

static int joypad_status[2][10];

static int joyreleaseval(int column, int *status)
{
    int val = 0;

    switch (column) {
      case 1:
        val = (status[2] ? 0 : 0x2) | (status[4] ? 0 : 0x4);
        break;
      case 3:
        val = (status[2] ? 0 : 0x2) | (status[6] ? 0 : 0x8);
        break;
      case 7:
        val = (status[4] ? 0 : 0x4) | (status[8] ? 0 : 0x1);
        break;
      case 9:
        val = (status[8] ? 0 : 0x1) | (status[6] ? 0 : 0x8);
        break;
      default:
        val = joypad_bits[column];
        break;
    }
    return ~val;
}

int joystick_check_set(signed long key, int joynum)
{
    int column;
    unsigned int joyport;

    if (joystick_port_map[0] == joynum)
        joyport = 1;
    else if (joystick_port_map[1] == joynum)
        joyport = 2;
    else
        return 0;

    joynum--;

    for (column = 0; column < 10; column++) {
        if (key == joykeys[joynum][column].sym) {
            if (joypad_bits[column]) {
                /*joystick_value[joyport] |= joypad_bits[column];*/
                joystick_set_value_or(joyport, (BYTE)joypad_bits[column]);
                joypad_status[joynum][column] = 1;
            } else {
                /*joystick_value[joyport] = 0;*/
                joystick_set_value_absolute(joyport, 0);
                memset(joypad_status[joynum], 0, sizeof(joypad_status[joynum]));
            }
            return 1;
        }
    }
    return 0;
}

int joystick_check_clr(signed long key, int joynum)
{
    int column;
    unsigned int joyport;

    if (joystick_port_map[0] == joynum)
        joyport = 1;
    else if (joystick_port_map[1] == joynum)
        joyport = 2;
    else
        return 0;

    joynum--;

    for (column = 0; column < 10; column++) {
        if (key == joykeys[joynum][column].sym) {
            /*joystick_value[joyport] &= joyreleaseval(column,
                                                     joypad_status[joynum]);*/
            joystick_set_value_and(joyport, (BYTE)joyreleaseval(column,
                                   joypad_status[joynum]));
            joypad_status[joynum][column] = 0;
            return 1;
        }
    }
    return 0;
}

void joystick_joypad_clear(void)
{
    memset(joypad_status, 0, sizeof(joypad_status));
}
#endif

/*-----------------------------------------------------------------------*/

int joystick_init(void)
{
    joystick_alarm = alarm_new(maincpu_alarm_context, "Joystick",
                               joystick_latch_handler, NULL);

    return joy_arch_init();
}

/*--------------------------------------------------------------------------*/
int joystick_snapshot_write_module(snapshot_t *s)
{
    snapshot_module_t *m;

    m = snapshot_module_create(s, "JOYSTICK", 1, 0);
    if (m == NULL)
       return -1;

    if (0
        || SMW_BA(m, joystick_value, JOYSTICK_NUM) < 0)
    {
        snapshot_module_close(m);
        return -1;
    }

    if (snapshot_module_close(m) < 0)
        return -1;

    return 0;
}

int joystick_snapshot_read_module(snapshot_t *s)
{
    BYTE major_version, minor_version;
    snapshot_module_t *m;

    m = snapshot_module_open(s, "JOYSTICK",
                             &major_version, &minor_version);
    if (m == NULL) {
        return 0;
    }

    if (0
        || SMR_BA(m, joystick_value, JOYSTICK_NUM) < 0)
    {
        snapshot_module_close(m);
        return -1;
    }

    snapshot_module_close(m);
    return 0;
}
