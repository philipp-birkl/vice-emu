
/*
 * pruser.h - Printer device for userport.
 *
 * Written by
 *  Andr� Fachat <a.fachat@physik.tu-chemnitz.de>
 *
 * Patches by
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

#ifndef _PRUSER_H_
#define _PRUSER_H_

#include "types.h"

extern int pruser_init_resources(void);
extern int pruser_init_cmdline_options(void);

extern void pruser_write_data(BYTE);
extern void pruser_write_strobe(int);
extern void pruser_set_busy(int);

#endif

