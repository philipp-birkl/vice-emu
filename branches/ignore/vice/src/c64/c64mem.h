/*
 * c64mem.h -- C64 memory handling.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Andreas Boose <boose@linux.rz.fh-hannover.de>
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

#ifndef _C64MEM_H
#define _C64MEM_H

#include "mem.h"
#include "types.h"

#define C64_RAM_SIZE                    0x10000
#define C64_KERNAL_ROM_SIZE             0x2000
#define C64_BASIC_ROM_SIZE              0x2000
#define C64_CHARGEN_ROM_SIZE            0x1000

#define C64_BASIC_CHECKSUM              15702
#define C64_KERNAL_CHECKSUM_R00         50955
#define C64_KERNAL_CHECKSUM_R03         50954
#define C64_KERNAL_CHECKSUM_R03swe      50633
#define C64_KERNAL_CHECKSUM_R43         50955
#define C64_KERNAL_CHECKSUM_R64         49680

extern int c64_mem_init_resources(void);
extern int c64_mem_init_cmdline_options(void);
extern void mem_set_vbank(int new_vbank);
extern read_func_t ram_read;
extern read_func_t roml_read, romh_read;
extern read_func_t ultimax_a000_bfff_read;
extern store_func_t ram_store, ram_hi_store;
extern store_func_t roml_store, ultimax_a000_bfff_store;

extern char** mem_get_romsets(void);
extern int mem_get_numromsets(void);
extern char* mem_get_romset_name(void);
extern int mem_romset_loader(void);
extern int mem_set_romset(char *name);
extern int mem_add_romset(char *name);

extern void pla_config_changed(void);
extern void mem_set_tape_sense(int sense);
extern void mem_set_exrom(int active);

extern int mem_load_kernal(const char *rom_name);
extern int mem_load_basic(const char *rom_name);
extern int mem_load_chargen(const char *rom_name);

#endif /* _C64MEM_H */

