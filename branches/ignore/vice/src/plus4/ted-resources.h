/*
 * ted-resources.h - Resources for the TED emulation.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Ettore Perazzoli <ettore@comm2000.it>
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

#ifndef _TED_RESOURCES_H
#define _TED_RESOURCES_H

#include "vice.h"

#include "tedtypes.h"

/* VIC-II resources.  */
struct ted_resources_s
{
#ifdef VIC_II_NEED_2X
    /* Flag: Do we use double size?  */
    int double_size_enabled;

    /* Flag: Do we copy lines in double size mode?  */
    int double_scan_enabled;
#endif

#ifdef USE_XF86_EXTENSIONS
    /* Flag: Do we use double size?  */
    int fullscreen_double_size_enabled;

    /* Flag: Do we copy lines in double size mode?  */
    int fullscreen_double_scan_enabled;
#endif
};
typedef struct ted_resources_s ted_resources_t;

extern ted_resources_t ted_resources;

extern int ted_resources_init(void);

#endif

