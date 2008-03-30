/*
 * event.c - Event handling.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alarm.h"
#include "archdep.h"
#include "cmdline.h"
#include "event.h"
#include "interrupt.h"
#include "keyboard.h"
#include "log.h"
#include "machine.h"
#include "maincpu.h"
#include "resources.h"
#include "snapshot.h"
#include "types.h"
#include "utils.h"


#define EVENT_START_SNAPSHOT "start" FSDEV_EXT_SEP_STR "vsf"
#define EVENT_END_SNAPSHOT "end" FSDEV_EXT_SEP_STR "vsf"


struct event_list_s {
    unsigned int type;
    CLOCK clk;
    unsigned int size;
    void *data;
    struct event_list_s *next;
};
typedef struct event_list_s event_list_t;


static event_list_t *event_list_base, *event_list_current;

static alarm_t event_alarm;

static log_t event_log = LOG_DEFAULT;

static unsigned int event_playback_active = 0, event_record_active = 0;

static char *event_start_snapshot = NULL;
static char *event_end_snapshot = NULL;

void event_record(unsigned int type, void *data, unsigned int size)
{
    void *event_data = NULL;

    if (event_record_active == 0)
        return;

    switch (type) {
      case EVENT_KEYBOARD_MATRIX:
        event_data = xmalloc(size);
        memcpy(event_data, data, size);
        break;
      default:
        log_error(event_log, "Unknow event type %i.", type);
        return;
    }

    event_list_current->type = type;
    event_list_current->clk = maincpu_clk;
    event_list_current->size = size;
    event_list_current->data = event_data;
    event_list_current->next
        = (event_list_t *)xcalloc(1, sizeof(event_list_t));
    event_list_current = event_list_current->next;
}

static void event_alarm_handler(CLOCK offset)
{
    alarm_unset(&event_alarm);

    switch (event_list_current->type) {
      case EVENT_KEYBOARD_MATRIX:
        keyboard_event_playback(offset, event_list_current->data);
        break;
      default:
        log_error(event_log, "Unknow event type %i.", event_list_current->type);
    }

    event_list_current = event_list_current->next;

    if (event_list_current->next != NULL)
        alarm_set(&event_alarm, event_list_current->clk);
}

/*-----------------------------------------------------------------------*/

static void create_list(void)
{
    event_list_base = (event_list_t *)xcalloc(1, sizeof(event_list_t));
    event_list_current = event_list_base;
}

static void destroy_list(void)
{
    event_list_t *c1, *c2;

    c1 = event_list_base;

    while (c1 != NULL) {
        c2 = c1->next;
        free(c1->data);
        free(c1);
        c1 = c2;
    } 

    event_list_base = NULL;
    event_list_current = NULL;
}

/*-----------------------------------------------------------------------*/

static void event_record_start_trap(ADDRESS addr, void *data)
{
    machine_write_snapshot(event_start_snapshot, 1, 1, 0);

    destroy_list();
    create_list();

    event_record_active = 1;
}

int event_record_start(void)
{
    if (event_playback_active != 0)
        return -1;

    if (event_record_active != 0)
        return -1;

    interrupt_maincpu_trigger_trap(event_record_start_trap, (void *)0);

    return 0;
}

static void event_record_stop_trap(ADDRESS addr, void *data)
{
    machine_write_snapshot(event_end_snapshot, 1, 1, 1);

    event_record_active = 0;
}

int event_record_stop(void)
{
    if (event_record_active == 0)
        return -1;

    interrupt_maincpu_trigger_trap(event_record_stop_trap, (void *)0);

    return 0;
}

static void event_playback_start_trap(ADDRESS addr, void *data)
{
    snapshot_t *s;
    BYTE minor, major;

    if (machine_read_snapshot(event_start_snapshot, 0) < 0)
        return;

    s = snapshot_open(event_end_snapshot, &major, &minor, machine_name);

    if (s == NULL)
        return;

    destroy_list();
    create_list();

    if (event_snapshot_read_module(s, 1) < 0) {
        snapshot_close(s);
        return;
    }

    snapshot_close(s);

    event_list_current = event_list_base;

    event_playback_active = 1;

    if (event_list_current->next != NULL)
        alarm_set(&event_alarm, event_list_current->clk);
}


int event_playback_start(void)
{
    if (event_record_active != 0)
        return -1;

    if (event_playback_active != 0)
        return -1;

    interrupt_maincpu_trigger_trap(event_playback_start_trap, (void *)0);

    return 0;
}

int event_playback_stop(void)
{
    if (event_playback_active == 0)
        return -1;

    event_playback_active = 0;

    alarm_unset(&event_alarm);

    return 0;
}

/*-----------------------------------------------------------------------*/

static int read_dword_into_int(snapshot_module_t *m, int *value_return)
{
    DWORD b;

    if (snapshot_module_read_dword(m, &b) < 0)
        return -1;
    *value_return = (int)b;
    return 0;
}

int event_snapshot_read_module(struct snapshot_s *s, int event_mode)
{
    snapshot_module_t *m;
    BYTE major_version, minor_version;
    event_list_t *curr;

    if (event_mode == 0)
        return 0;

    m = snapshot_module_open(s, "EVENT", &major_version, &minor_version);

    /* This module is not mandatory.  */
    if (m == NULL)
        return 0;

    destroy_list();
    create_list();

    curr = event_list_base;

    while (1) {
        if (read_dword_into_int(m, &(curr->type)) < 0) {
            snapshot_module_close(m);
            return -1;
        }

        if (curr->type == EVENT_LIST_END)
            break;

        if (snapshot_module_read_dword(m, &(curr->clk)) < 0) {
            snapshot_module_close(m);
            return -1;
        }

        if (read_dword_into_int(m, &(curr->size)) < 0) {
            snapshot_module_close(m);
            return -1;
        }

        if (curr->size > 0) {
            curr->data = xmalloc(curr->size);
            if (snapshot_module_read_byte_array(m, curr->data, curr->size)
                < 0) {
                snapshot_module_close(m);
                return -1;
            }
        }         

        curr->next = (event_list_t *)xcalloc(1, sizeof(event_list_t));
        curr = curr->next;
    }

    snapshot_module_close(m);

    return 0;
}

int event_snapshot_write_module(struct snapshot_s *s, int event_mode)
{
    snapshot_module_t *m;
    event_list_t *curr;

    if (event_mode == 0)
        return 0;

    m = snapshot_module_create(s, "EVENT", 0, 0);
 
    if (m == NULL)
        return -1;

    curr = event_list_base;

    while (curr != NULL) {
        if (0
            || snapshot_module_write_dword(m, (DWORD)curr->type) < 0
            || snapshot_module_write_dword(m, (DWORD)curr->clk) < 0
            || snapshot_module_write_dword(m, (DWORD)curr->size) < 0
            || snapshot_module_write_byte_array(m, curr->data, curr->size)) {
            snapshot_module_close(m);
            return -1;
        } 
        curr = curr->next;
    }

    if (snapshot_module_write_dword(m, (DWORD)EVENT_LIST_END) < 0) {
        snapshot_module_close(m);
        return -1;
    }

    if (snapshot_module_close(m) < 0)
        return -1;

    return 0;
}

/*-----------------------------------------------------------------------*/

static int set_event_start_snapshot(resource_value_t v, void *param)
{
    if (util_string_set(&event_start_snapshot, (const char *)v))
        return 0;

    return 0;
}

static int set_event_end_snapshot(resource_value_t v, void *param)
{
    if (util_string_set(&event_end_snapshot, (const char *)v))
        return 0;

    return 0;
}

static resource_t resources[] = {
    { "EventStartSnapshot", RES_STRING, (resource_value_t)EVENT_START_SNAPSHOT,
      (resource_value_t *)&event_start_snapshot,
      set_event_start_snapshot, NULL },
    { "EventEndSnapshot", RES_STRING, (resource_value_t)EVENT_END_SNAPSHOT,
      (resource_value_t *)&event_end_snapshot,
      set_event_end_snapshot, NULL },
    { NULL }
};

int event_resources_init(void)
{
    return resources_register(resources);
}

/*-----------------------------------------------------------------------*/

static cmdline_option_t cmdline_options[] = {
    { NULL }
};

int event_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}

/*-----------------------------------------------------------------------*/

void event_init(void)
{
    event_log = log_open("Event");

    alarm_init(&event_alarm, maincpu_alarm_context,
               "Event", event_alarm_handler);
}

