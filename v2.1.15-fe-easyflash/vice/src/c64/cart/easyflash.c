/*
 * easyflash.c - Cartridge handling of the easyflash cart.
 *
 * Written by
 *  ALeX Kazik <alx@kazik.de>
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
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
#include <string.h>

#include "c64cart.h"
#include "c64cartmem.h"
#include "c64io.h"
#include "c64mem.h"
#include "cmdline.h"
#include "easyflash.h"
#include "flash040.h"
#include "lib.h"
#include "log.h"
#include "resources.h"
#include "translate.h"

/* the 27F040B statemachine */
static flash040_context_t *easyflash_state_low = NULL;
static flash040_context_t *easyflash_state_high = NULL;

/* the jumper */
static int easyflash_jumper;

/* writing back to crt enabled */
static int easyflash_crt_write;

/* backup of the registers */
static BYTE easyflash_register_00, easyflash_register_02;

/* decoding table of the modes */
static const BYTE easyflash_memconfig[] = {
           /* bit3 = jumper, bit2 = mode, bit1 = exrom, bit0 = game */

           /* jumper off, mode 0, trough 00,01,10,11 in game/exrom bits */
    4 + 3, /* exrom high, game = jumper = low */
    8 + 3, /* Reserved, don't use this */
    4 + 1, /* exrom high, game = jumper = high */
    8 + 1, /* Reserved, don't use this */

           /* jumper off, mode 1, trough 00,01,10,11 in game/exrom bits */
    0 + 2, 0 + 3, 0 + 0, 0 + 1,

           /* jumper on, mode 0, trough 00,01,10,11 in game/exrom bits */
    4 + 2, /* exrom high, game = jumper = low */
    8 + 3, /* Reserved, don't use this */
    4 + 0, /* exrom low, game = jumper = low */
    8 + 1, /* Reserved, don't use this */

           /* jumper on, mode 1, trough 00,01,10,11 in game/exrom bits */
    0 + 2, 0 + 3, 0 + 0, 0 + 1,
};

/*
   info:
   game=0, exrom=0 -> 16k
   game=1, exrom=0 -> 8k
   game=0, exrom=1 -> ultimax
   game=1, exrom=1 -> off
*/

/* extra RAM */
static BYTE easyflash_ram[256];

/* ---------------------------------------------------------------------*/

static int set_easyflash_jumper(int val, void *param)
{
    easyflash_jumper = val;
    return 0;
}

static int set_easyflash_crt_write(int val, void *param)
{
    easyflash_crt_write = val;
    return 0;
}

static const resource_int_t resources_int[] = {
    { "EasyFlashJumper", 0, RES_EVENT_STRICT, (resource_value_t)0,
      &easyflash_jumper, set_easyflash_jumper, NULL },
    { "EasyFlashWriteCRT", 0, RES_EVENT_NO, NULL,
      &easyflash_crt_write, set_easyflash_crt_write, NULL },
    { NULL }
};

int easyflash_resources_init(void)
{
    return resources_register_int(resources_int);
}

static const cmdline_option_t cmdline_options[] =
{
    { "-easyflashjumper", SET_RESOURCE, 0,
      NULL, NULL, "EasyFlashJumper", (resource_value_t)1,
      USE_PARAM_STRING, USE_DESCRIPTION_STRING,
      IDCLS_UNUSED, IDCLS_UNUSED,
      NULL, T_("Enable easyflash jumpper") },
    { "+easyflashjumper", SET_RESOURCE, 0,
      NULL, NULL, "EasyFlashJumper", (resource_value_t)0,
      USE_PARAM_STRING, USE_DESCRIPTION_STRING,
      IDCLS_UNUSED, IDCLS_UNUSED,
      NULL, T_("Disable easyflash jumper") },
    { "-easyflashcrtwrite", SET_RESOURCE, 0,
      NULL, NULL, "EasyFlashWriteCRT", (resource_value_t)1,
      USE_PARAM_STRING, USE_DESCRIPTION_STRING,
      IDCLS_UNUSED, IDCLS_UNUSED,
      NULL, T_("Enable writing to easyflash .crt image") },
    { "+easyflashcrtwrite", SET_RESOURCE, 0,
      NULL, NULL, "EasyFlashWriteCRT", (resource_value_t)0,
      USE_PARAM_STRING, USE_DESCRIPTION_STRING,
      IDCLS_UNUSED, IDCLS_UNUSED,
      NULL, T_("Disable writing to easyflash .crt image") },
    { NULL }
};

int easyflash_cmdline_options_init(void)
{
    return cmdline_register_options(cmdline_options);
}


/* ---------------------------------------------------------------------*/

void REGPARM2 easyflash_io1_store(WORD addr, BYTE value)
{
    BYTE mem_mode;

    switch (addr & 2) {
        case 0:
            /* bank register */
            easyflash_register_00 = value & 0x3f; /* we only remember 6 bits */
            break;
        default:
            /* mode register */
            easyflash_register_02 = value & 0x87; /* we only remember led, mode, exrom, game */
            mem_mode = easyflash_memconfig[(easyflash_jumper << 3) | (easyflash_register_02 & 0x07)];
            cartridge_config_changed(mem_mode, mem_mode, CMODE_READ);
            /* TODO: change led */
            /* (value & 0x80) -> led on if true, led off if false */
    }
}

BYTE REGPARM1 easyflash_io2_read(WORD addr)
{
    io_source = IO_SOURCE_EASYFLASH;
    return easyflash_ram[addr & 0xff];
}

void REGPARM2 easyflash_io2_store(WORD addr, BYTE value)
{
    easyflash_ram[addr & 0xff] = value;
}

BYTE REGPARM1 easyflash_roml_read(WORD addr)
{
    return flash040core_read(easyflash_state_low, (easyflash_register_00 * 0x2000) + (addr & 0x1fff));
}

void REGPARM2 easyflash_roml_store(WORD addr, BYTE value)
{
    flash040core_store(easyflash_state_low, (easyflash_register_00 * 0x2000) + (addr & 0x1fff), value);
}

BYTE REGPARM1 easyflash_romh_read(WORD addr)
{
    return flash040core_read(easyflash_state_high, (easyflash_register_00 * 0x2000) + (addr & 0x1fff));
}

void REGPARM2 easyflash_romh_store(WORD addr, BYTE value)
{
    flash040core_store(easyflash_state_high, (easyflash_register_00 * 0x2000) + (addr & 0x1fff), value);
}

void easyflash_config_init(void)
{
    cartridge_store_io1((WORD)0xde00, 0);
    cartridge_store_io1((WORD)0xde02, 0);
}

void easyflash_config_setup(BYTE *rawcart)
{
   easyflash_state_low = lib_malloc(sizeof(flash040_context_t));
   easyflash_state_high = lib_malloc(sizeof(flash040_context_t));

   flash040core_init(easyflash_state_low, FLASH040_TYPE_B);
   memcpy(easyflash_state_low->flash_data, rawcart, 0x80000);

   flash040core_init(easyflash_state_high, FLASH040_TYPE_B);
   memcpy(easyflash_state_high->flash_data, rawcart + 0x80000, 0x80000);
}

int easyflash_crt_attach(FILE *fd, BYTE *rawcart, BYTE *header)
{
    BYTE chipheader[0x10];

    while (1) {
        if (fread(chipheader, 0x10, 1, fd) < 1) {
            break;
        }
        if (chipheader[0xb] >= 64 || (chipheader[0xc] != 0x80 && chipheader[0xc] != 0xa0)) {
            return -1;
        }
        if (fread(&rawcart[(chipheader[0xb] << 13) | (chipheader[0xc] == 0x80 ? 0 : 1<<19)], 0x2000, 1, fd) < 1) {
            return -1;
        }
    }
    return 0;
}

void easyflash_detach(void)
{
    flash040core_shutdown(easyflash_state_low);
    flash040core_shutdown(easyflash_state_high);
    lib_free(easyflash_state_low);
    lib_free(easyflash_state_high);
}
