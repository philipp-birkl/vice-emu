/*
 * vic20-resources.h
 *
 * Written by
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

#ifndef _VIC20_RESOURCES_H
#define _VIC20_RESOURCES_H

extern int vic20_init_resources(void);

extern int emu_id_enabled;
extern int ieee488_enabled;
extern int mem_rom_blocks;
extern int ram_block_0_enabled;
extern int ram_block_1_enabled;
extern int ram_block_2_enabled;
extern int ram_block_3_enabled;
extern int ram_block_5_enabled;

#endif

