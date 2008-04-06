/*
 * c610cpu.c - Emulation of the main 6510 processor.
 *
 * Written by
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

#include "vice.h"

#include "mem.h"
#include "types.h"

/* MACHINE_STUFF should define/undef

 - NEED_REG_PC
 - TRACE
 - JUMP

 The following are optional:

 - PAGE_ZERO
 - PAGE_ONE
 - STORE_IND
 - LOAD_IND

*/

/* ------------------------------------------------------------------------- */

extern read_func_ptr_t *_mem_read_ind_tab_ptr;
extern store_func_ptr_t *_mem_write_ind_tab_ptr;

extern BYTE *page_zero;
extern BYTE *page_one;
#  define       PAGE_ZERO       page_zero
#  define       PAGE_ONE        page_one

#  define STORE_IND(addr, value) \
    (*_mem_write_ind_tab_ptr[(addr) >> 8])((ADDRESS)(addr), (BYTE)(value))

#  define LOAD_IND(addr) \
    (*_mem_read_ind_tab_ptr[(addr) >> 8])((ADDRESS)(addr))

#include "../maincpu.c"

