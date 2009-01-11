/*
 * stb.h - Cartridge handling, Structured Basic cart.
 *
 * Written by
 *  Walter Zimmer, adapted from kcs.h
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

#ifndef _STB_H
#define _STB_H

#include <stdio.h>

#include "types.h"

extern BYTE REGPARM1 stb_io1_read(WORD addr);
extern void REGPARM2 stb_io1_store(WORD addr, BYTE value);

extern void stb_config_init(void);
extern void stb_config_setup(BYTE *rawcart);
extern int stb_bin_attach(const char *filename, BYTE *rawcart);
extern int stb_crt_attach(FILE *fd, BYTE *rawcart);
extern void stb_detach(void);

#endif
