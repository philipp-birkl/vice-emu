/*
 * event.h - Event handling.
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

#ifndef _EVENT_H
#define _EVENT_H

#define EVENT_LIST_END          0
#define EVENT_KEYBOARD_MATRIX   1
#define EVENT_KEYBOARD_RESTORE  2
#define EVENT_JOYSTICK_VALUE    3
#define EVENT_DATASETTE         4
#define EVENT_INITIAL           5
#define EVENT_ATTACHDISK        6
#define EVENT_ATTACHTAPE        7
#define EVENT_RESETCPU          8
#define EVENT_TIMESTAMP         9

#define EVENT_START_MODE_FILE_SAVE 0
#define EVENT_START_MODE_FILE_LOAD 1
#define EVENT_START_MODE_RESET     2

struct snapshot_s;

extern void event_init(void);
extern int event_resources_init(void);
extern void event_shutdown(void);
extern int event_cmdline_options_init(void);

extern int event_record_start(void);
extern int event_record_stop(void);
extern int event_playback_start(void);
extern int event_playback_stop(void);
extern int event_record_active(void);
extern int event_playback_active(void);
extern int event_record_set_milestone(void);
extern int event_record_reset_milestone(void);

extern void event_reset_ack(void);

extern void event_record(unsigned int type, void *data, unsigned int size);

extern int event_snapshot_read_module(struct snapshot_s *s, int event_mode);
extern int event_snapshot_write_module(struct snapshot_s *s, int event_mode);

#endif

