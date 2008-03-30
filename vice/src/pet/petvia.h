/*
 * petvia.h - PET VIA emulation.
 *
 * Written by
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
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

#ifndef _PET_VIA_H
#define _PET_VIA_H

#include "types.h"

struct snapshot_s;

extern void via_reset(void);
extern void via_init(void);
extern BYTE REGPARM1 via_read(WORD addr);
extern BYTE REGPARM1 via_peek(WORD addr);
extern void REGPARM2 via_store(WORD addr, BYTE value);

extern int via_snapshot_read_module(struct snapshot_s *);
extern int via_snapshot_write_module(struct snapshot_s *);

#endif

