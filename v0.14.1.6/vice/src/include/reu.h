/*
 * reu.h - REU 1750 emulation.
 *
 * Written by
 *  Jouko Valta (jopi@stekt.oulu.fi)
 *  Richard Hable (K3027E7@edvz.uni-linz.ac.at)
 *
 * Fixes by
 *  Ettore Perazzoli (ettore@comm2000.it) [EP]
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

#ifndef _REU_H
#define _REU_H

#include "types.h"

extern int reset_reu(const char *file_name, int size);
extern void activate_reu(void);
extern void close_reu(const char *file_name);
extern BYTE REGPARM1 read_reu(ADDRESS addr);
extern void REGPARM2 store_reu(ADDRESS addr, BYTE byte);
extern void reu_dma(int immed);

#endif
