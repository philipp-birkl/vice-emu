/*
 * mouse.h - Common mouse handling
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *
 * NEOS and Amiga mouse support by
 *  H.Nuotio <hannu.nuotio@pp.inet.fi>
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

#ifndef _MOUSE_H
#define _MOUSE_H

#include "types.h"

extern int mouse_resources_init(void);
extern int mouse_cmdline_options_init(void);
extern void mouse_init(void);

extern void mouse_button_left(int pressed);
extern void mouse_button_right(int pressed);

extern BYTE mouse_get_x(void);
extern BYTE mouse_get_y(void);

extern int _mouse_enabled;
extern int mouse_type;
extern int mouse_port;

extern void neos_mouse_store(BYTE val);
extern BYTE neos_mouse_read(void);
extern BYTE amiga_mouse_read(void);

#define MOUSE_TYPE_1351   0
#define MOUSE_TYPE_NEOS   1
#define MOUSE_TYPE_AMIGA  2

#endif
