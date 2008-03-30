/*
 * c64mem.c -- C64 memory handling.
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

#include "vice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "archdep.h"
#include "c64-resources.h"
#include "c64cart.h"
#include "c64cia.h"
#include "c64mem.h"
#include "c64memlimit.h"
#include "cartridge.h"
#include "cmdline.h"
#include "datasette.h"
#include "emuid.h"
#include "interrupt.h"
#include "log.h"
#include "maincpu.h"
#include "mon.h"
#include "parallel.h"
#include "patchrom.h"
#include "resources.h"
#include "reu.h"
#include "sid.h"
#include "snapshot.h"
#include "sysfile.h"
#include "ui.h"
#include "utils.h"
#include "vicii-mem.h"
#include "vicii.h"

#ifdef HAVE_RS232
#include "c64acia.h"
#endif

/* ------------------------------------------------------------------------- */

/* C64 memory-related resources.  */

/* Adjust this pointer when the MMU changes banks.  */
static BYTE **bank_base;
static int *bank_limit = NULL;
unsigned int old_reg_pc;

const char *mem_romset_resources_list[] = {
    "KernalName", "ChargenName", "BasicName",
    "CartridgeType", "CartridgeFile",
    "DosName2031", "DosName1001",
    "DosName1541", "DosName1571", "DosName1581", "DosName1541ii",
    NULL
};

/* ------------------------------------------------------------------------- */

/* Number of possible memory configurations.  */
#define NUM_CONFIGS     32
/* Number of possible video banks (16K each).  */
#define NUM_VBANKS      4

/* The C64 memory.  */
BYTE ram[C64_RAM_SIZE];
BYTE basic_rom[C64_BASIC_ROM_SIZE];
BYTE kernal_rom[C64_KERNAL_ROM_SIZE];
BYTE chargen_rom[C64_CHARGEN_ROM_SIZE];

/* Size of RAM...  */
int ram_size = C64_RAM_SIZE;

/* Flag: nonzero if the Kernal and BASIC ROMs have been loaded.  */
static int rom_loaded = 0;

/* Pointers to the currently used memory read and write tables.  */
read_func_ptr_t *_mem_read_tab_ptr;
store_func_ptr_t *_mem_write_tab_ptr;
BYTE **_mem_read_base_tab_ptr;
int *mem_read_limit_tab_ptr;

/* Memory read and write tables.  */
static store_func_ptr_t mem_write_tab[NUM_VBANKS][NUM_CONFIGS][0x101];
static read_func_ptr_t mem_read_tab[NUM_CONFIGS][0x101];
static BYTE *mem_read_base_tab[NUM_CONFIGS][0x101];
static int mem_read_limit_tab[NUM_CONFIGS][0x101];

static store_func_ptr_t mem_write_tab_watch[0x101];
static read_func_ptr_t mem_read_tab_watch[0x101];

static store_func_ptr_t mem_write_tab_orig[NUM_VBANKS][NUM_CONFIGS][0x101];
static read_func_ptr_t mem_read_tab_orig[NUM_CONFIGS][0x101];

/* Processor port.  */
static struct {
    BYTE dir, data, data_out;
} pport;

/* Current video bank (0, 1, 2 or 3).  */
static int vbank;

/* Current memory configuration.  */
static int mem_config;

/* Tape sense status: 1 = some button pressed, 0 = no buttons pressed.  */
static int tape_sense = 0;

/* Tape motor status.  */
static BYTE old_port_data_out = 0xff;

/* Tape write line status.  */
static BYTE old_port_write_bit = 0xff;

/* Logging goes here.  */
static log_t c64_mem_log = LOG_ERR;

/* ------------------------------------------------------------------------- */

BYTE REGPARM1 read_watch(ADDRESS addr)
{
    mon_watch_push_load_addr(addr, e_comp_space);
    return mem_read_tab[mem_config][addr >> 8](addr);
}


void REGPARM2 store_watch(ADDRESS addr, BYTE value)
{
    mon_watch_push_store_addr(addr, e_comp_space);
    mem_write_tab[vbank][mem_config][addr >> 8](addr, value);
}

/* ------------------------------------------------------------------------- */

inline void pla_config_changed(void)
{
    mem_config = (((~pport.dir | pport.data) & 0x7) | (export.exrom << 3)
                  | (export.game << 4));

    pport.data_out = (pport.data_out & ~pport.dir)
                     | (pport.data & pport.dir);

    ram[1] = ((pport.data | ~pport.dir) & (pport.data_out | 0x17));

    if (!(pport.dir & 0x20))
      ram[1] &= 0xdf;

    if (tape_sense && !(pport.dir & 0x10))
      ram[1] &= 0xef;

    if (((pport.dir & pport.data) & 0x20) != old_port_data_out) {
        old_port_data_out = (pport.dir & pport.data) & 0x20;
        datasette_set_motor(!old_port_data_out);
    }

    if (((~pport.dir | pport.data) & 0x8) != old_port_write_bit) {
        old_port_write_bit = (~pport.dir | pport.data) & 0x8;
        datasette_toggle_write_bit((~pport.dir | pport.data) & 0x8);
    }

    ram[0] = pport.dir;

    if (any_watchpoints(e_comp_space)) {
        _mem_read_tab_ptr = mem_read_tab_watch;
        _mem_write_tab_ptr = mem_write_tab_watch;
    } else {
        _mem_read_tab_ptr = mem_read_tab[mem_config];
        _mem_write_tab_ptr = mem_write_tab[vbank][mem_config];
    }

    _mem_read_base_tab_ptr = mem_read_base_tab[mem_config];
    mem_read_limit_tab_ptr = mem_read_limit_tab[mem_config];

    if (bank_limit != NULL) {
        *bank_base = _mem_read_base_tab_ptr[old_reg_pc >> 8];
        if (*bank_base != 0)
            *bank_base = _mem_read_base_tab_ptr[old_reg_pc >> 8]
                         - (old_reg_pc & 0xff00);
        *bank_limit = mem_read_limit_tab_ptr[old_reg_pc >> 8];
    }
}

void mem_toggle_watchpoints(int flag)
{
    if (flag) {
        _mem_read_tab_ptr = mem_read_tab_watch;
        _mem_write_tab_ptr = mem_write_tab_watch;
    } else {
        _mem_read_tab_ptr = mem_read_tab[mem_config];
        _mem_write_tab_ptr = mem_write_tab[vbank][mem_config];
    }
}

BYTE REGPARM1 read_zero(ADDRESS addr)
{
    return ram[addr & 0xff];
}

void REGPARM2 store_zero(ADDRESS addr, BYTE value)
{
    addr &= 0xff;

    switch ((BYTE) addr) {
      case 0:
        if (pport.dir != value) {
            pport.dir = value;
            pla_config_changed();
        }
        break;
      case 1:
        if (pport.data != value) {
            pport.data = value;
            pla_config_changed();
        }
        break;
      default:
        ram[addr] = value;
    }
}

BYTE REGPARM1 basic_read(ADDRESS addr)
{
    return basic_rom[addr & 0x1fff];
}

BYTE REGPARM1 kernal_read(ADDRESS addr)
{
    return kernal_rom[addr & 0x1fff];
}

BYTE REGPARM1 chargen_read(ADDRESS addr)
{
    return chargen_rom[addr & 0xfff];
}

BYTE REGPARM1 ram_read(ADDRESS addr)
{
    return ram[addr];
}

void REGPARM2 ram_store(ADDRESS addr, BYTE value)
{
    ram[addr] = value;
}

void REGPARM2 ram_hi_store(ADDRESS addr, BYTE value)
{
    ram[addr] = value;

    if (addr == 0xff00)
        reu_dma(-1);
}

void REGPARM2 io2_store(ADDRESS addr, BYTE value)
{
    if (mem_cartridge_type != CARTRIDGE_NONE)
        cartridge_store_io2(addr, value);
    if (reu_enabled)
        reu_store((ADDRESS)(addr & 0x0f), value);
    return;
}

BYTE REGPARM1 io2_read(ADDRESS addr)
{
    if (mem_cartridge_type != CARTRIDGE_NONE)
        return cartridge_read_io2(addr);
    if (emu_id_enabled && addr >= 0xdfa0) {
        addr &= 0xff;
        if (addr == 0xff)
            emulator_id[addr - 0xa0] ^= 0xff;
        return emulator_id[addr - 0xa0];
    }
    if (reu_enabled)
        return reu_read((ADDRESS)(addr & 0x0f));
    return rand();
}

void REGPARM2 io1_store(ADDRESS addr, BYTE value)
{
    if (mem_cartridge_type != CARTRIDGE_NONE)
        cartridge_store_io1(addr, value);
#ifdef HAVE_RS232
    if (acia_de_enabled)
        acia1_store(addr & 0x03, value);
#endif
    return;
}

BYTE REGPARM1 io1_read(ADDRESS addr)
{
    if (mem_cartridge_type != CARTRIDGE_NONE)
        return cartridge_read_io1(addr);
#ifdef HAVE_RS232
    if (acia_de_enabled)
        return acia1_read(addr & 0x03);
#endif
    return rand();
}

BYTE REGPARM1 rom_read(ADDRESS addr)
{
    switch (addr & 0xf000) {
      case 0xa000:
      case 0xb000:
        return basic_read(addr);
      case 0xd000:
        return chargen_read(addr);
      case 0xe000:
      case 0xf000:
        return kernal_read(addr);
    }

    return 0;
}

void REGPARM2 rom_store(ADDRESS addr, BYTE value)
{
    switch (addr & 0xf000) {
      case 0xa000:
      case 0xb000:
        basic_rom[addr & 0x1fff] = value;
        break;
      case 0xd000:
        chargen_rom[addr & 0x0fff] = value;
        break;
      case 0xe000:
      case 0xf000:
        kernal_rom[addr & 0x1fff] = value;
        break;
    }
}

/* ------------------------------------------------------------------------- */

static void REGPARM2 cartridge_decode_store(ADDRESS addr, BYTE value)
{
    /*
     * Enable cartridge first before making an access.
     */
    cartridge_decode_address(addr);

    /*
     * Store data at the decoded address.
     */
    mem_write_tab_orig[vbank][mem_config][addr >> 8](addr, value);
}

static BYTE REGPARM1 cartridge_decode_read(ADDRESS addr)
{
    /*
     * Enable cartridge first before making an access.
     */
    cartridge_decode_address(addr);

    /*
     * Read data from decoded address.
     */
    return mem_read_tab_orig[mem_config][addr >> 8](addr);
}

/* ------------------------------------------------------------------------- */

/* Generic memory access.  */

void REGPARM2 mem_store(ADDRESS addr, BYTE value)
{
    _mem_write_tab_ptr[addr >> 8](addr, value);
}

BYTE REGPARM1 mem_read(ADDRESS addr)
{
    return _mem_read_tab_ptr[addr >> 8](addr);
}

/* ------------------------------------------------------------------------- */

static void set_write_hook(int config, int page, store_func_t * f)
{
    int i;

    for (i = 0; i < NUM_VBANKS; i++) {
        mem_write_tab[i][config][page] = f;
    }
}

void mem_initialize_memory(void)
{
    int i, j, k;
    /* IO is enabled at memory configs 5, 6, 7 and Ultimax.  */
    int io_config[32] = { 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1 };
    /* ROML is enabled at memory configs 11, 15, 27, 31 and Ultimax.  */
    int roml_config[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
                            1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    /* ROMH is enabled at memory configs 10, 11, 14, 15, 26, 27, 30, 31
       and Ultimax.  */
    int romh_config[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                            1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 };
    /* ROMH is mapped to $A000-$BFFF at memory configs 10, 11, 14, 15, 26,
       27, 30, 31.  If Ultimax is enabled it is mapped to $E000-$FFFF.  */
    int romh_mapping[32] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
                             0x00, 0x00, 0xa0, 0xa0, 0x00, 0x00, 0xa0, 0xa0 };

    mem_limit_init(mem_read_limit_tab);

/*
    if (c64_mem_log == LOG_ERR)
        c64_mem_log = log_open("C64MEM");
*/
    /* Default is RAM.  */
    for (i = 0; i <= 0x100; i++) {
        mem_read_tab_watch[i] = read_watch;
        mem_write_tab_watch[i] = store_watch;
    }

    for (i = 0; i < NUM_CONFIGS; i++) {
        set_write_hook(i, 0, store_zero);
        mem_read_tab[i][0] = read_zero;
        mem_read_base_tab[i][0] = ram;
        for (j = 1; j <= 0xfe; j++) {
            mem_read_tab[i][j] = ram_read;
            mem_read_base_tab[i][j] = ram + (j << 8);
            for (k = 0; k < NUM_VBANKS; k++) {
                if ((j & 0xc0) == (k << 6)) {
                    switch (j & 0x3f) {
                      case 0x39:
                        mem_write_tab[k][i][j] = vicii_mem_vbank_39xx_store;
                        break;
                      case 0x3f:
                        mem_write_tab[k][i][j] = vicii_mem_vbank_3fxx_store;
                        break;
                      default:
                        mem_write_tab[k][i][j] = vicii_mem_vbank_store;
                    }
                } else {
                    mem_write_tab[k][i][j] = ram_store;
                }
            }
        }
        mem_read_tab[i][0xff] = ram_read;
        mem_read_base_tab[i][0xff] = ram + 0xff00;

        /* FIXME: we do not care about vbank writes here, but we probably
           should.  Anyway, the $FFxx addresses are not so likely to contain
           sprites or other stuff that really needs the special handling, and
           it's much easier this way.  */
        set_write_hook(i, 0xff, ram_hi_store);
    }

    /* Setup BASIC ROM at $A000-$BFFF (memory configs 3, 7, 11, 15).  */
    for (i = 0xa0; i <= 0xbf; i++) {
        mem_read_tab[3][i] = basic_read;
        mem_read_tab[7][i] = basic_read;
        mem_read_tab[11][i] = basic_read;
        mem_read_tab[15][i] = basic_read;
        mem_read_base_tab[3][i] = basic_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[7][i] = basic_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[11][i] = basic_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[15][i] = basic_rom + ((i & 0x1f) << 8);
    }

    /* Setup character generator ROM at $D000-$DFFF (memory configs 1, 2,
       3, 9, 10, 11, 25, 26, 27).  */
    for (i = 0xd0; i <= 0xdf; i++) {
        mem_read_tab[1][i] = chargen_read;
        mem_read_tab[2][i] = chargen_read;
        mem_read_tab[3][i] = chargen_read;
        mem_read_tab[9][i] = chargen_read;
        mem_read_tab[10][i] = chargen_read;
        mem_read_tab[11][i] = chargen_read;
        mem_read_tab[25][i] = chargen_read;
        mem_read_tab[26][i] = chargen_read;
        mem_read_tab[27][i] = chargen_read;
        mem_read_base_tab[1][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[2][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[3][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[9][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[10][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[11][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[25][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[26][i] = chargen_rom + ((i & 0x0f) << 8);
        mem_read_base_tab[27][i] = chargen_rom + ((i & 0x0f) << 8);
    }

    /* Setup I/O at $D000-$DFFF (memory configs 5, 6, 7).  */
    for (j = 0; j < NUM_CONFIGS; j++) {
        if (io_config[j]) {
            for (i = 0xd0; i <= 0xd3; i++) {
                mem_read_tab[j][i] = vic_read;
                set_write_hook(j, i, vic_store);
            }
            for (i = 0xd4; i <= 0xd5; i++) {
                mem_read_tab[j][i] = sid_read;
                set_write_hook(j, i, sid_store);
            }
            for (i = 0xd6; i <= 0xd7; i++) {
                mem_read_tab[j][i] = sid_read;
                set_write_hook(j, i, sid_store);
            }
            for (i = 0xd8; i <= 0xdb; i++) {
                mem_read_tab[j][i] = colorram_read;
                set_write_hook(j, i, colorram_store);
            }

            mem_read_tab[j][0xdc] = cia1_read;
            set_write_hook(j, 0xdc, cia1_store);
            mem_read_tab[j][0xdd] = cia2_read;
            set_write_hook(j, 0xdd, cia2_store);

            mem_read_tab[j][0xde] = io1_read;
            set_write_hook(j, 0xde, io1_store);
            mem_read_tab[j][0xdf] = io2_read;
            set_write_hook(j, 0xdf, io2_store);

            for (i = 0xd0; i <= 0xdf; i++)
                mem_read_base_tab[j][i] = NULL;
        }
    }

    /* Setup Kernal ROM at $E000-$FFFF (memory configs 2, 3, 6, 7, 10,
       11, 14, 15, 26, 27, 30, 31).  */
    for (i = 0xe0; i <= 0xff; i++) {
        mem_read_tab[2][i] = kernal_read;
        mem_read_tab[3][i] = kernal_read;
        mem_read_tab[6][i] = kernal_read;
        mem_read_tab[7][i] = kernal_read;
        mem_read_tab[10][i] = kernal_read;
        mem_read_tab[11][i] = kernal_read;
        mem_read_tab[14][i] = kernal_read;
        mem_read_tab[15][i] = kernal_read;
        mem_read_tab[26][i] = kernal_read;
        mem_read_tab[27][i] = kernal_read;
        mem_read_tab[30][i] = kernal_read;
        mem_read_tab[31][i] = kernal_read;
        mem_read_base_tab[2][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[3][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[6][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[7][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[10][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[11][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[14][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[15][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[26][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[27][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[30][i] = kernal_rom + ((i & 0x1f) << 8);
        mem_read_base_tab[31][i] = kernal_rom + ((i & 0x1f) << 8);
    }

    /* Setup ROML at $8000-$9FFF.  */
    for (j = 0; j < NUM_CONFIGS; j++) {
        if (roml_config[j]) {
            for (i = 0x80; i <= 0x9f; i++) {
                mem_read_tab[j][i] = roml_read;
                mem_read_base_tab[j][i] = NULL;
            }
        }
    }
    for (j = 16; j < 24; j++) {
        for (i = 0x80; i <= 0x9f; i++)
            set_write_hook(j, i, roml_store);
        for (i = 0xa0; i <= 0xbf; i++) {
            mem_read_tab[j][i] = ultimax_a000_bfff_read;
            set_write_hook(j, i, ultimax_a000_bfff_store);
        }
    }

    /* Setup ROMH at $A000-$BFFF and $E000-$FFFF.  */
    for (j = 0; j < NUM_CONFIGS; j++) {
        if (romh_config[j]) {
            for (i = romh_mapping[j]; i <= (romh_mapping[j] + 0x1f); i++) {
                mem_read_tab[j][i] = romh_read;
                mem_read_base_tab[j][i] = NULL;
            }
        }
    }

    for (i = 0; i < NUM_CONFIGS; i++) {
        mem_read_tab[i][0x100] = mem_read_tab[i][0];
        for (j = 0; j < NUM_VBANKS; j++) {
            mem_write_tab[j][i][0x100] = mem_write_tab[j][i][0];
        }
        mem_read_base_tab[i][0x100] = mem_read_base_tab[i][0];
    }

    _mem_read_tab_ptr = mem_read_tab[7];
    _mem_write_tab_ptr = mem_write_tab[vbank][7];
    _mem_read_base_tab_ptr = mem_read_base_tab[7];
    mem_read_limit_tab_ptr = mem_read_limit_tab[7];

    pport.data = 0x37;
    pport.dir = 0x2f;
    export.exrom = 0;
    export.game = 0;

    /*
     * Copy mapping to a separate table.
     * This table will contain the original (unmodified) mapping.
     */
    memcpy(mem_write_tab_orig, mem_write_tab,
           NUM_VBANKS*sizeof(*mem_write_tab));
    memcpy(mem_read_tab_orig, mem_read_tab,
           NUM_CONFIGS*sizeof(*mem_read_tab));

    /*
     * Change address decoding.
     */
    if (mem_cartridge_type == CARTRIDGE_EXPERT) {

        /*
         * Mapping for ~GAME disabled
         */
        for (j = 0; j < 16; j++) {
            for (i = 0x80; i <= 0x9f; i++) {
                /* $8000 - $9FFF */
                mem_read_tab[j][i] = cartridge_decode_read;

                /* Setup writing at $8000-$9FFF */
                set_write_hook(j, i, cartridge_decode_store);

                /* $E000 - $FFFF */
                mem_read_tab[j][i+0x60] = cartridge_decode_read;
                /* Setup writing at $E000-$FFFF */
                set_write_hook(j, i+0x60, cartridge_decode_store);
            }
        }

        /*
         * Mapping for ~GAME enabled.
         */
        for (j = 16; j < NUM_CONFIGS; j++) {
            /* $0000 - $7FFF */
            for (i = 0x00; i <= 0x7f; i++) {
                /* Setup reading */
                mem_read_tab[j][i] = cartridge_decode_read;

                /* Setup writing */
                set_write_hook(j, i, cartridge_decode_store);
            }

            /* $A000 - $FFFF */
            for (i = 0xa0; i <= 0xff; i++) {
                /* Setup reading */
                mem_read_tab[j][i] = cartridge_decode_read;

                /* Setup writing */
                set_write_hook(j, i, cartridge_decode_store);
            }
        }
    }

    /* Setup initial memory configuration.  */
    pla_config_changed();
    cartridge_init_config();
}

/* ------------------------------------------------------------------------- */

/* Initialize RAM for power-up.  */
void mem_powerup(void)
{
    int i;

    for (i = 0; i < 0x10000; i += 0x80) {
        memset(ram + i, 0, 0x40);
        memset(ram + i + 0x40, 0xff, 0x40);
    }
}

static int c64mem_get_kernal_checksum(void)
{
    int i;
    WORD sum;                   /* ROM checksum */
    int id;                     /* ROM identification number */

    /* Check Kernal ROM.  */
    for (i = 0, sum = 0; i < C64_KERNAL_ROM_SIZE; i++)
        sum += kernal_rom[i];

    id = rom_read(0xff80);

    log_message(c64_mem_log, "Kernal rev #%d.", id);

    if ((id == 0
         && sum != C64_KERNAL_CHECKSUM_R00)
        || (id == 3
            && sum != C64_KERNAL_CHECKSUM_R03
            && sum != C64_KERNAL_CHECKSUM_R03swe)
        || (id == 0x43
            && sum != C64_KERNAL_CHECKSUM_R43)
        || (id == 0x64
            && sum != C64_KERNAL_CHECKSUM_R64)) {
        log_warning(c64_mem_log,
                    "Warning: Unknown Kernal image.  Sum: %d ($%04X).",
                    sum, sum);
    } else if (kernal_revision != NULL) {
        if (patch_rom(kernal_revision) < 0)
            return -1;
    }
/*
    {
        int drive_true_emulation;
        resources_get_value("DriveTrueEmulation",
                            (resource_value_t) &drive_true_emulation);
        if (!drive_true_emulation)
            serial_install_traps();
    }
*/
    return 0;
}

int mem_load_kernal(const char *rom_name)
{
    int trapfl;

    if (!rom_loaded)
        return 0;

    /* Make sure serial code assumes there are no traps installed.  */
    /* serial_remove_traps(); */
    /* we also need the TAPE traps!!! therefore -> */
    /* disable traps before saving the ROM */
    resources_get_value("VirtualDevices", (resource_value_t*) &trapfl);
    resources_set_value("VirtualDevices", (resource_value_t) 1);

    /* Load Kernal ROM.  */
    if (sysfile_load(rom_name,
        kernal_rom, C64_KERNAL_ROM_SIZE,
        C64_KERNAL_ROM_SIZE) < 0) {
        log_error(c64_mem_log, "Couldn't load kernal ROM `%s'.",
                  rom_name);
        resources_set_value("VirtualDevices", (resource_value_t) trapfl);
        return -1;
    }
    c64mem_get_kernal_checksum();

    resources_set_value("VirtualDevices", (resource_value_t) trapfl);

    return 0;
}

static int c64mem_get_basic_checksum(void)
{
    int i;
    WORD sum;

    /* Check Basic ROM.  */

    for (i = 0, sum = 0; i < C64_BASIC_ROM_SIZE; i++)
        sum += basic_rom[i];

    if (sum != C64_BASIC_CHECKSUM)
        log_warning(c64_mem_log,
                    "Warning: Unknown Basic image.  Sum: %d ($%04X).",
                    sum, sum);

    return 0;
}

int mem_load_basic(const char *rom_name)
{
    if (!rom_loaded)
        return 0;

    /* Load Basic ROM.  */
    if (sysfile_load(rom_name,
        basic_rom, C64_BASIC_ROM_SIZE,
        C64_BASIC_ROM_SIZE) < 0) {
        log_error(c64_mem_log,
                  "Couldn't load basic ROM `%s'.",
                  rom_name);
        return -1;
    }
    return c64mem_get_basic_checksum();
}


int mem_load_chargen(const char *rom_name)
{
    if (!rom_loaded)
        return 0;

    /* Load chargen ROM.  */

    if (sysfile_load(rom_name,
        chargen_rom, C64_CHARGEN_ROM_SIZE,
        C64_CHARGEN_ROM_SIZE) < 0) {
        log_error(c64_mem_log, "Couldn't load character ROM `%s'.",
                  rom_name);
        return -1;
    }

    return 0;
}


int mem_load(void)
{
    char *rom_name = NULL;

    mem_powerup();

    if (c64_mem_log == LOG_ERR)
        c64_mem_log = log_open("C64MEM");

    rom_loaded = 1;

    if (resources_get_value("KernalName", (resource_value_t)&rom_name) < 0)
        return -1;
    if (mem_load_kernal(rom_name) < 0)
        return -1;

    if (resources_get_value("BasicName", (resource_value_t)&rom_name) < 0)
        return -1;
    if (mem_load_basic(rom_name) < 0)
        return -1;

    if (resources_get_value("ChargenName", (resource_value_t)&rom_name) < 0)
        return -1;
    if (mem_load_chargen(rom_name) < 0)
        return -1;

    return 0;
}

/* ------------------------------------------------------------------------- */

/* Change the current video bank.  Call this routine only when the vbank
   has really changed.  */
void mem_set_vbank(int new_vbank)
{
    vbank = new_vbank;
    _mem_write_tab_ptr = mem_write_tab[new_vbank][mem_config];
    vic_ii_set_vbank(new_vbank);
}

/* Set the tape sense status.  */
void mem_set_tape_sense(int sense)
{
    tape_sense = sense;
    pla_config_changed();
}

/* Enable/disable the REU.  FIXME: should initialize the REU if necessary?  */
void mem_toggle_reu(int flag)
{
    reu_enabled = flag;
}

/* Enable/disable the Emulator ID.  */
void mem_toggle_emu_id(int flag)
{
    emu_id_enabled = flag;
}

void mem_set_bank_pointer(BYTE **base, int *limit)
{
    bank_base = base;
    bank_limit = limit;
}

/* ------------------------------------------------------------------------- */

/* FIXME: this part needs to be checked.  */

void mem_get_basic_text(ADDRESS * start, ADDRESS * end)
{
    if (start != NULL)
        *start = ram[0x2b] | (ram[0x2c] << 8);
    if (end != NULL)
        *end = ram[0x2d] | (ram[0x2e] << 8);
}

void mem_set_basic_text(ADDRESS start, ADDRESS end)
{
    ram[0x2b] = ram[0xac] = start & 0xff;
    ram[0x2c] = ram[0xad] = start >> 8;
    ram[0x2d] = ram[0x2f] = ram[0x31] = ram[0xae] = end & 0xff;
    ram[0x2e] = ram[0x30] = ram[0x32] = ram[0xaf] = end >> 8;
}

/* ------------------------------------------------------------------------- */

int mem_rom_trap_allowed(ADDRESS addr)
{
    return addr >= 0xe000 && (mem_config & 0x2);
}

/* ------------------------------------------------------------------------- */

/* Banked memory access functions for the monitor.  */

/* FIXME: peek, cartridge support */

static void store_bank_io(ADDRESS addr, BYTE byte)
{
    switch (addr & 0xff00) {
      case 0xd000:
      case 0xd100:
      case 0xd200:
      case 0xd300:
        vic_store(addr, byte);
        break;
      case 0xd400:
      case 0xd500:
        sid_store(addr, byte);
        break;
      case 0xd600:
      case 0xd700:
        sid_store(addr, byte);
        break;
      case 0xd800:
      case 0xd900:
      case 0xda00:
      case 0xdb00:
        colorram_store(addr, byte);
        break;
      case 0xdc00:
        cia1_store(addr, byte);
        break;
      case 0xdd00:
        cia2_store(addr, byte);
        break;
      case 0xde00:
        io1_store(addr, byte);
        break;
      case 0xdf00:
        io2_store(addr, byte);
        break;
    }
    return;
}

static BYTE read_bank_io(ADDRESS addr)
{
    switch (addr & 0xff00) {
      case 0xd000:
      case 0xd100:
      case 0xd200:
      case 0xd300:
        return vic_read(addr);
      case 0xd400:
      case 0xd500:
      case 0xd600:
      case 0xd700:
        return sid_read(addr);
      case 0xd800:
      case 0xd900:
      case 0xda00:
      case 0xdb00:
        return colorram_read(addr);
      case 0xdc00:
        return cia1_read(addr);
      case 0xdd00:
        return cia2_read(addr);
      case 0xde00:
        return io1_read(addr);
      case 0xdf00:
        return io2_read(addr);
    }
    return 0xff;
}

static BYTE peek_bank_io(ADDRESS addr)
{
    switch (addr & 0xff00) {
      case 0xd000:
      case 0xd100:
      case 0xd200:
      case 0xd300:
        return vic_peek(addr);
      case 0xd400:
      case 0xd500:
      case 0xd600:
      case 0xd700:
        return sid_read(addr);
      case 0xd800:
      case 0xd900:
      case 0xda00:
      case 0xdb00:
        return colorram_read(addr);
      case 0xdc00:
        return cia1_peek(addr);
      case 0xdd00:
        return cia2_peek(addr);
      case 0xde00:
        return io1_read(addr);  /* FIXME */
      case 0xdf00:
        return io2_read(addr);  /* FIXME */
    }
    return 0xff;
}

/* ------------------------------------------------------------------------- */

/* Exported banked memory access functions for the monitor.  */

static const char *banknames[] =
{
    "default", "cpu", "ram", "rom", "io", "cart", NULL
};

static int banknums[] =
{
    1, 0, 1, 2, 3, 4
};

const char **mem_bank_list(void)
{
    return banknames;
}

int mem_bank_from_name(const char *name)
{
    int i = 0;

    while (banknames[i]) {
        if (!strcmp(name, banknames[i])) {
            return banknums[i];
        }
        i++;
    }
    return -1;
}

BYTE mem_bank_read(int bank, ADDRESS addr)
{
    switch (bank) {
      case 0:                   /* current */
        return mem_read(addr);
        break;
      case 3:                   /* io */
        if (addr >= 0xd000 && addr < 0xe000) {
            return read_bank_io(addr);
        }
      case 4:                   /* cart */
        if (addr >= 0x8000 && addr <= 0x9fff) {
            return roml_banks[addr & 0x1fff];
        }
        if (addr >= 0xa000 && addr <= 0xbfff) {
            return romh_banks[addr & 0x1fff];
        }
      case 2:                   /* rom */
        if (addr >= 0xa000 && addr <= 0xbfff) {
            return basic_rom[addr & 0x1fff];
        }
        if (addr >= 0xd000 && addr <= 0xdfff) {
            return chargen_rom[addr & 0x0fff];
        }
        if (addr >= 0xe000 && addr <= 0xffff) {
            return kernal_rom[addr & 0x1fff];
        }
      case 1:                   /* ram */
        break;
    }
    return ram[addr];
}

BYTE mem_bank_peek(int bank, ADDRESS addr)
{
    switch (bank) {
      case 0:                   /* current */
        return mem_read(addr);  /* FIXME */
        break;
      case 3:                   /* io */
        if (addr >= 0xd000 && addr < 0xe000) {
            return peek_bank_io(addr);
        }
    }
    return mem_bank_read(bank, addr);
}

void mem_bank_write(int bank, ADDRESS addr, BYTE byte)
{
    switch (bank) {
      case 0:                   /* current */
        mem_store(addr, byte);
        return;
      case 3:                   /* io */
        if (addr >= 0xd000 && addr < 0xe000) {
            store_bank_io(addr, byte);
            return;
        }
      case 2:                   /* rom */
        if (addr >= 0xa000 && addr <= 0xbfff) {
            return;
        }
        if (addr >= 0xd000 && addr <= 0xdfff) {
            return;
        }
        if (addr >= 0xe000 && addr <= 0xffff) {
            return;
        }
      case 1:                   /* ram */
        break;
    }
    ram[addr] = byte;
}

mem_ioreg_list_t *mem_ioreg_list_get(void)
{
    mem_ioreg_list_t *mem_ioreg_list;

    mem_ioreg_list = (mem_ioreg_list_t *)xmalloc(sizeof(mem_ioreg_list_t) * 4);

    mem_ioreg_list[0].name = "VIC-II";
    mem_ioreg_list[0].start = 0xd000;
    mem_ioreg_list[0].end = 0xd02e;
    mem_ioreg_list[0].next = &mem_ioreg_list[1];

    mem_ioreg_list[1].name = "SID";
    mem_ioreg_list[1].start = 0xd400;
    mem_ioreg_list[1].end = 0xd41f;
    mem_ioreg_list[1].next = &mem_ioreg_list[2];

    mem_ioreg_list[2].name = "CIA1";
    mem_ioreg_list[2].start = 0xdc00;
    mem_ioreg_list[2].end = 0xdc0f;
    mem_ioreg_list[2].next = &mem_ioreg_list[3];

    mem_ioreg_list[3].name = "CIA2";
    mem_ioreg_list[3].start = 0xdd00;
    mem_ioreg_list[3].end = 0xdd0f;
    mem_ioreg_list[3].next = NULL;

    return mem_ioreg_list;
}

void mem_get_screen_parameter(ADDRESS *base, BYTE *rows, BYTE *columns)
{
    *base = ((vic_peek(0xd018) & 0xf0) << 6)
            | ((~cia2_peek(0xdd00) & 0x03) << 14);
    *rows = 25;
    *columns = 40;
}

/* ------------------------------------------------------------------------- */

void mem_set_exrom(int active)
{
    export.exrom = active ? 0 : 1;

    pla_config_changed();
}

/* ------------------------------------------------------------------------- */

/* Snapshot.  */

#define SNAP_ROM_MAJOR 0
#define SNAP_ROM_MINOR 0
static const char snap_rom_module_name[] = "C64ROM";

static int mem_write_rom_snapshot_module(snapshot_t *s)
{
    snapshot_module_t *m;
    int trapfl;

    /* Main memory module.  */

    m = snapshot_module_create(s, snap_rom_module_name,
                                        SNAP_ROM_MAJOR, SNAP_ROM_MINOR);
    if (m == NULL)
        return -1;

    /* disable traps before saving the ROM */
    resources_get_value("VirtualDevices", (resource_value_t*) &trapfl);
    resources_set_value("VirtualDevices", (resource_value_t) 1);

    if (snapshot_module_write_byte_array(m, kernal_rom,
                                                C64_KERNAL_ROM_SIZE) < 0
        || snapshot_module_write_byte_array(m, basic_rom,
                                                C64_BASIC_ROM_SIZE) < 0
        || snapshot_module_write_byte_array(m, chargen_rom,
                                                C64_CHARGEN_ROM_SIZE) < 0
        )
        goto fail;

    /* FIXME: save cartridge ROM (& RAM?) areas:
       first write out the configuration, i.e.
       - type of cartridge (banking scheme type)
       - state of cartridge (active/which bank, ...)
       then the ROM/RAM arrays:
       - cartridge ROM areas
       - cartridge RAM areas  */

    ui_update_menus();

    if (snapshot_module_close(m) < 0)
        goto fail;

    resources_set_value("VirtualDevices", (resource_value_t) trapfl);

    return 0;

fail:
    if (m != NULL)
        snapshot_module_close(m);

    resources_set_value("VirtualDevices", (resource_value_t) trapfl);

    return -1;
}

int mem_read_rom_snapshot_module(snapshot_t *s)
{
    BYTE major_version, minor_version;
    snapshot_module_t *m;
    int trapfl;

    /* Main memory module.  */

    m = snapshot_module_open(s, snap_rom_module_name,
                             &major_version, &minor_version);
    if (m == NULL) {
        /* this module is optional */
        /* FIXME: reset all cartridge stuff to standard C64 behaviour */
        return 0;
    }

    if (major_version > SNAP_ROM_MAJOR || minor_version > SNAP_ROM_MINOR) {
        log_error(c64_mem_log,
                  "Snapshot module version (%d.%d) newer than %d.%d.",
                  major_version, minor_version,
                  SNAP_ROM_MAJOR, SNAP_ROM_MINOR);
        snapshot_module_close(m);
        return -1;
    }

    /* disable traps before loading the ROM */
    resources_get_value("VirtualDevices", (resource_value_t*) &trapfl);
    resources_set_value("VirtualDevices", (resource_value_t) 1);

    if (snapshot_module_read_byte_array(m, kernal_rom,
                                                C64_KERNAL_ROM_SIZE) < 0
        || snapshot_module_read_byte_array(m, basic_rom,
                                                C64_BASIC_ROM_SIZE) < 0
        || snapshot_module_read_byte_array(m, chargen_rom,
                                                C64_CHARGEN_ROM_SIZE) < 0
        )
        goto fail;

    /* FIXME: read cartridge ROM (& RAM?) areas:
       first read out the configuration, i.e.
       - type of cartridge (banking scheme type)
       - state of cartridge (active/which bank, ...)
       then the ROM/RAM arrays:
       - cartridge ROM areas
       - cartridge RAM areas
    */

    if (snapshot_module_close(m) < 0)
        goto fail;

    c64mem_get_kernal_checksum();
    c64mem_get_basic_checksum();
    /* enable traps again when necessary */
    resources_set_value("VirtualDevices", (resource_value_t) trapfl);


    return 0;

fail:
    if (m != NULL)
        snapshot_module_close(m);
    resources_set_value("VirtualDevices", (resource_value_t) trapfl);
    return -1;
}


#define SNAP_MAJOR 0
#define SNAP_MINOR 0
static const char snap_mem_module_name[] = "C64MEM";

int mem_write_snapshot_module(snapshot_t *s, int save_roms)
{
    snapshot_module_t *m;

    /* Main memory module.  */

    m = snapshot_module_create(s, snap_mem_module_name, SNAP_MAJOR, SNAP_MINOR);    if (m == NULL)
        return -1;

    if (snapshot_module_write_byte(m, pport.data) < 0
        || snapshot_module_write_byte(m, pport.dir) < 0
        || snapshot_module_write_byte(m, export.exrom) < 0
        || snapshot_module_write_byte(m, export.game) < 0
        || snapshot_module_write_byte_array(m, ram, C64_RAM_SIZE) < 0)
        goto fail;

    if (snapshot_module_close(m) < 0)
        goto fail;
    m = NULL;

    if (save_roms && mem_write_rom_snapshot_module(s) < 0)
        goto fail;

    /* REU module.  */
    if (reu_enabled && reu_write_snapshot_module(s) < 0)
        goto fail;

#ifdef HAVE_RS232
    /* ACIA module.  */
    if (acia_de_enabled && acia1_write_snapshot_module(s) < 0)
        goto fail;
#endif

    return 0;

fail:
    if (m != NULL)
        snapshot_module_close(m);
    return -1;
}

int mem_read_snapshot_module(snapshot_t *s)
{
    BYTE major_version, minor_version;
    snapshot_module_t *m;

    /* Main memory module.  */

    m = snapshot_module_open(s, snap_mem_module_name,
                             &major_version, &minor_version);
    if (m == NULL)
        return -1;

    if (major_version > SNAP_MAJOR || minor_version > SNAP_MINOR) {
        log_error(c64_mem_log,
                  "Snapshot module version (%d.%d) newer than %d.%d.",
                  major_version, minor_version,
                  SNAP_MAJOR, SNAP_MINOR);
        goto fail;
    }

    if (snapshot_module_read_byte(m, &pport.data) < 0
        || snapshot_module_read_byte(m, &pport.dir) < 0
        || snapshot_module_read_byte(m, &export.exrom) < 0
        || snapshot_module_read_byte(m, &export.game) < 0
        || snapshot_module_read_byte_array(m, ram, C64_RAM_SIZE) < 0)
        goto fail;

    pla_config_changed();

    if (snapshot_module_close(m) < 0)
        goto fail;
    m = NULL;

    if (mem_read_rom_snapshot_module(s) < 0)
        goto fail;

    /* REU module.  */
    if (reu_read_snapshot_module(s) < 0) {
        reu_enabled = 0;
    } else {
        reu_enabled = 1;
    }

#ifdef HAVE_RS232
    /* ACIA module.  */
    if (acia1_read_snapshot_module(s) < 0) {
        acia_de_enabled = 0;
    } else {
        /* FIXME: Why do we need to do so???  */
        acia1_reset();          /* Clear interrupts.  */
        acia_de_enabled = 1;
    }
#endif

    ui_update_menus();

    return 0;

fail:
    if (m != NULL)
        snapshot_module_close(m);
    return -1;
}

