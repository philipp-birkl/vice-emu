/*
 * c64cartmem.c -- C64 cartridge handling.
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

#include "vice.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "c64cart.h"
#include "c64mem.h"
#include "c64tpi.h"
#include "cartridge.h"
#include "interrupt.h"
#include "log.h"
#include "vicii.h"
#include "types.h"

/* #define DEBUG */

/* Expansion port signals.  */
export_t export;

/* Exansion port ROML/ROMH images.  */
#ifdef AVOID_STATIC_ARRAYS
BYTE *roml_banks, *romh_banks;
#else
BYTE roml_banks[0x20000], romh_banks[0x20000];
#endif

/* Exansion port RAM images.  */
#ifdef AVOID_STATIC_ARRAYS
BYTE *export_ram0;
#else
BYTE export_ram0[C64CART_RAM_LIMIT];
#endif

/* Expansion port ROML/ROMH/RAM banking.  */
int roml_bank = 0, romh_bank = 0, export_ram = 0;

/* Flag: Ultimax (VIC-10) memory configuration enabled.  */
int ultimax = 0;

/* Super Snapshot configuration flags.  */
static BYTE ramconfig = 0xff, romconfig = 9;
static int ram_bank = 0;		/* Version 5 supports 4 - 8Kb RAM banks. */

/* Atomic Power RAM hack.  */
static int export_ram_at_a000 = 0;

/* Type of the cartridge attached.  */
int mem_cartridge_type = CARTRIDGE_NONE;

/* Cartridge mode. Expert cartridge only (at the moment..) */
extern int cartmode; 

/*
 * Remember whether or not the cartridge should be activated or not.
 */
static int enable_trigger = 0;          /* Needed by Expert cartridge... */


static void cartridge_config_changed(BYTE mode)
{
    export.game = mode & 1;
    export.exrom = ((mode >> 1) & 1) ^ 1;
    romh_bank = roml_bank = (mode >> 3) & 3;
    export_ram = (mode >> 5) & 1;
    pla_config_changed();
    if (mode & 0x40)
        cartridge_release_freeze();
    ultimax = export.game & (export.exrom ^ 1);
    vic_ii_update_memory_ptrs_external();
}

void REGPARM1 cartridge_decode_address(ADDRESS addr)
{
	BYTE config;

	switch (mem_cartridge_type)
		{
		case CARTRIDGE_EXPERT:
			/*
			 * Mask A15-A13.
			 */
			addr = addr & 0xe000;

			/*
			 * Default disable ~GAME, ~EXROM & export_ram
			 * and do not mirror $8000-$9fff at $e000-$ffff in ultimax mode.
			 */
			config = (1 << 1);
			export_ram_at_a000 = 0;

			switch (cartmode)
				{
				case CARTRIDGE_MODE_ON:
					if (enable_trigger &&
						((addr == 0x8000) || (addr = 0xe000)))
						{
						config = (1 << 0);		/* Enable ~GAME */
						config |= (1 << 1);		/* Disable ~EXROM */
						config |= (1 << 5);		/* Enable export_ram */
						/*
						 * In ultimax mode: Mirror $8000-$9fff at $e000-$ffff
						 */
						export_ram_at_a000 = 1;
						}
					break;

				case CARTRIDGE_MODE_PRG:
					if (addr == 0x8000)
						{
						config = (1 << 0);		/* Enable ~GAME */
						config |= (1 << 1);		/* Disable ~EXROM */
						config |= (1 << 5);		/* Enable export_ram */
						/*
						 * In ultimax mode: Mirror $8000-$9fff at $e000-$ffff
						 */
						export_ram_at_a000 = 1;
						}
					break;
				}

			if (ramconfig != config)
				{
				cartridge_config_changed(config);
				ramconfig = config;
				}
			break;
		}
}

BYTE REGPARM1 cartridge_read_io1(ADDRESS addr)
{
#ifdef DEBUG
    log_debug("Read IO1 %02x.", addr);
#endif
    switch (mem_cartridge_type) {
      case CARTRIDGE_ACTION_REPLAY:
      case CARTRIDGE_ATOMIC_POWER:
        return rand();
      case CARTRIDGE_KCS_POWER:
        cartridge_config_changed(0);
        return roml_banks[0x1e00 + (addr & 0xff)];
      case CARTRIDGE_FINAL_III:
        return roml_banks[0x1e00 + (roml_bank << 13) + (addr & 0xff)];
      case CARTRIDGE_FINAL_I:
        cartridge_config_changed(0x42);
        return roml_banks[0x1e00 + (addr & 0xff)];
      case CARTRIDGE_SIMONS_BASIC:
        cartridge_config_changed(0);
        return rand();
      case CARTRIDGE_SUPER_SNAPSHOT:
        return export_ram0[0x1e00 + (addr & 0xff)];
      case CARTRIDGE_SUPER_SNAPSHOT_V5:
        switch (roml_bank) {
          case 0:
            return roml_banks[0x1e00 + (addr & 0xff)];
          case 1:
            return roml_banks[0x1e00 + (addr & 0xff) + 0x2000];
          case 2:
            return roml_banks[0x1e00 + (addr & 0xff) + 0x4000];
          case 3:
            return roml_banks[0x1e00 + (addr & 0xff) + 0x6000];
        }
	  case CARTRIDGE_EXPERT:
		switch (cartmode)
			{
			case CARTRIDGE_MODE_PRG:
			case CARTRIDGE_MODE_OFF:
				return 0;
			case CARTRIDGE_MODE_ON:
				/*
				 * Reset the nmi/reset trigger.
				 */
				enable_trigger = 0;
				return 0;
			}
	}
    return rand();
}

void REGPARM2 cartridge_store_io1(ADDRESS addr, BYTE value)
{
    int banknr;
 
#ifdef DEBUG
    log_debug("Store IO1 %02x <- %02x.", addr, value);
#endif

    switch (mem_cartridge_type) {
      case CARTRIDGE_ACTION_REPLAY:
        cartridge_config_changed(value);
        break;
      case CARTRIDGE_ATOMIC_POWER:
        if (value == 0x22) {
            value = 0x03;
            export_ram_at_a000 = 1;
        } else {
            export_ram_at_a000 = 0;
        }
        cartridge_config_changed(value);
        break;
      case CARTRIDGE_KCS_POWER:
        cartridge_config_changed(1);
        break;
      case CARTRIDGE_FINAL_I:
        cartridge_config_changed(0x42);
        break;
      case CARTRIDGE_SIMONS_BASIC:
        cartridge_config_changed(1);
        break;
      case CARTRIDGE_SUPER_SNAPSHOT:
        export_ram0[0x1e00 + (addr & 0xff)] = value;
        break;
      case CARTRIDGE_SUPER_SNAPSHOT_V5:
        if (((addr & 0xff) == 0)
	    || ((addr & 0xff) == 1)) {

            if ((value & 1) == 1) {
                cartridge_release_freeze();
            }

            /* D0 = ~GAME */
            romconfig = ((value & 1) ^ 1);

			/* Calc RAM/ROM bank nr. */
			banknr = ((value >> 2) & 0x1) | ((value >> 3) & 0x2);

			/* ROM ~OE set? */
			if (((value >> 3) & 1) == 0) {
				romconfig |= (banknr << 3);		/* Select ROM banknr. */
			}
				
			/* RAM ~OE set? */
			if (((value >> 1) & 1) == 0) {
				ram_bank = banknr;				/* Select RAM banknr. */
				romconfig |= (1 << 5);  		/* export_ram */
				romconfig |= (1 << 1); 			/* exrom */
			}
            cartridge_config_changed(romconfig);
        }
        break;
      case CARTRIDGE_OCEAN:
      case CARTRIDGE_FUNPLAY:
        switch (mem_cartridge_type) {
          case CARTRIDGE_OCEAN:
            romh_bank = roml_bank = value & 15;
            break;
          case CARTRIDGE_FUNPLAY:
            romh_bank = roml_bank = ((value >> 2) | (value & 1)) & 15;
            break;
        }
        if (value & 0x80) {
            export.game = (value >> 4) & 1;
            export.exrom = 1;
        } else {
            export.game = export.exrom = 1;
        }
        pla_config_changed();
        ultimax = 0;
        break;
	  case CARTRIDGE_EXPERT:
		switch (cartmode)
			{
			case CARTRIDGE_MODE_PRG:
			case CARTRIDGE_MODE_OFF:
				break;
			case CARTRIDGE_MODE_ON:
				/*
				 * Reset the nmi/reset trigger (= cartridge disabled).
				 */
				enable_trigger = 0;
				break;
			}   
    }
    return;
}

BYTE REGPARM1 cartridge_read_io2(ADDRESS addr)
{
#ifdef DEBUG
    log_debug("Read IO2 %02x.", addr);
#endif
    switch (mem_cartridge_type) {
      case CARTRIDGE_ACTION_REPLAY:
      case CARTRIDGE_ATOMIC_POWER:
      case CARTRIDGE_SUPER_SNAPSHOT:
      case CARTRIDGE_FINAL_III:
        if (mem_cartridge_type == CARTRIDGE_SUPER_SNAPSHOT
                               && (addr & 0xff) == 1)
            return ramconfig;
        if (export_ram && (mem_cartridge_type == CARTRIDGE_ACTION_REPLAY
            || mem_cartridge_type == CARTRIDGE_ATOMIC_POWER))
            return export_ram0[0x1f00 + (addr & 0xff)];
        switch (roml_bank) {
          case 0:
            return roml_banks[addr & 0x1fff];
          case 1:
            return roml_banks[(addr & 0x1fff) + 0x2000];
          case 2:
            return roml_banks[(addr & 0x1fff) + 0x4000];
          case 3:
            return roml_banks[(addr & 0x1fff) + 0x6000];
        }
        break;
      case CARTRIDGE_FINAL_I:
        cartridge_config_changed(1);
        return roml_banks[0x1f00 + (addr & 0xff)];
      case CARTRIDGE_KCS_POWER:
        return export_ram0[0x1f00 + (addr & 0xff)];
      case CARTRIDGE_IEEE488:
        return tpi_read((ADDRESS)(addr & 0x07));
      case CARTRIDGE_EPYX_FASTLOAD:
        if (addr == 0xdf18)
            cartridge_config_changed(0);
        if (addr == 0xdf38)
            cartridge_config_changed(2);
        return roml_banks[0x1f00 + (addr & 0xff)];
      case CARTRIDGE_WESTERMANN:
        cartridge_config_changed(0);
        return rand();
      case CARTRIDGE_REX:
        if ((addr & 0xff) < 0xc0)
            cartridge_config_changed(2);
        else
            cartridge_config_changed(0);
        return 0;
    }
    return rand();
}

void REGPARM2 cartridge_store_io2(ADDRESS addr, BYTE value)
{
#ifdef DEBUG
    log_debug("Store IO2 %02x <- %02x.", addr, value);
#endif
    switch (mem_cartridge_type) {
      case CARTRIDGE_ACTION_REPLAY:
      case CARTRIDGE_ATOMIC_POWER:
        if (export_ram)
            export_ram0[0x1f00 + (addr & 0xff)] = value;
        break;
      case CARTRIDGE_FINAL_I:
        cartridge_config_changed(1);
        break;
      case CARTRIDGE_KCS_POWER:
        export_ram0[0x1f00 + (addr & 0xff)] = value;
        break;
      case CARTRIDGE_SUPER_SNAPSHOT:
        if ((addr & 0xff) == 0) {
            romconfig = (value == 2) ? 1 : 9;
            romconfig = (romconfig & 0xdf) | ((ramconfig == 0) ? 0x20 : 0);
            if ((value & 0x7f) == 0)
                romconfig = 35;
            if ((value & 0x7f) == 1 || (value & 0x7f) == 3)
                romconfig = 0;
            if ((value & 0x7f) == 6) {
                romconfig = 9;
                cartridge_release_freeze();
            }
            if ((value & 0x7f) == 9)
                romconfig = 6;
            cartridge_config_changed(romconfig);
        }
        if ((addr & 0xff) == 1) {
            if(((ramconfig - 1) & 0xff) == value) {
                ramconfig = value;
                romconfig |= 35;
            }
            if(((ramconfig + 1) & 0xff) == value) {
                ramconfig = value;
                romconfig &= 0xdd;
            }
            cartridge_config_changed(romconfig);
        }
        break;
      case CARTRIDGE_FINAL_III:
        if ((addr & 0xff) == 0xff)  {
            /* FIXME: Change this to call `cartridge_config_changed'.  */
            romh_bank = roml_bank = value & 3;
            export.game = ((value >> 5) & 1) ^ 1;
            export.exrom = ((value >> 4) & 1) ^ 1;
            pla_config_changed();
            ultimax = export.game & (export.exrom ^ 1);
            if ((value & 0x30) == 0x10)
                maincpu_set_nmi(I_FREEZE, IK_NMI);
            if (value & 0x40)
                cartridge_release_freeze();
        }
        break;
      case CARTRIDGE_SUPER_GAMES:
        romh_bank = roml_bank = value & 3;
        if (value & 0x4) {
            export.game = 0;
            export.exrom = 1;
        } else {
            export.game = export.exrom = 1;
        }
        if (value == 0xc)
            export.game = export.exrom = 0;
        pla_config_changed();
        break;
      case CARTRIDGE_IEEE488:
        tpi_store((ADDRESS)(addr & 0x07), value);
        break;
    }
    return;
}

BYTE REGPARM1 read_roml(ADDRESS addr)
{
    if (export_ram) {
        if (mem_cartridge_type == CARTRIDGE_SUPER_SNAPSHOT_V5)
            return export_ram0[(addr & 0x1fff) + (ram_bank << 13)];
        else 
            return export_ram0[addr & 0x1fff];
    }
    return roml_banks[(addr & 0x1fff) + (roml_bank << 13)];
}

BYTE REGPARM1 read_romh(ADDRESS addr)
{
    if (export_ram_at_a000)
        return export_ram0[addr & 0x1fff];
    return romh_banks[(addr & 0x1fff) + (romh_bank << 13)];
}

void REGPARM2 store_roml(ADDRESS addr, BYTE value)
{
    if (export_ram) {
        if (mem_cartridge_type == CARTRIDGE_SUPER_SNAPSHOT_V5)
            export_ram0[(addr & 0x1fff) + (ram_bank << 13)] = value;
        else
            export_ram0[addr & 0x1fff] = value;
    }
    return;
}

BYTE REGPARM1 read_ultimax_a000_bfff(ADDRESS addr)
{
    if (export_ram_at_a000)
        return export_ram0[addr & 0x1fff];
    return 0x55;
}

void REGPARM2 store_ultimax_a000_bfff(ADDRESS addr, BYTE value)
{
    if (export_ram_at_a000)
        export_ram0[addr & 0x1fff] = value;
    return;
}

void cartridge_init_config(void)
{
    export_ram_at_a000 = 0;
    switch (mem_cartridge_type) {
      case CARTRIDGE_ACTION_REPLAY:
      case CARTRIDGE_ATOMIC_POWER:
      case CARTRIDGE_KCS_POWER:
      case CARTRIDGE_GENERIC_8KB:
      case CARTRIDGE_SUPER_GAMES:
      case CARTRIDGE_EPYX_FASTLOAD:
      case CARTRIDGE_REX:
        cartridge_config_changed(0);
        break;
      case CARTRIDGE_FINAL_I:
      case CARTRIDGE_FINAL_III:
      case CARTRIDGE_SIMONS_BASIC:
      case CARTRIDGE_GENERIC_16KB:
      case CARTRIDGE_WESTERMANN:
        cartridge_config_changed(1);
        break;
      case CARTRIDGE_ULTIMAX:
        cartridge_config_changed(3);
        break;
      case CARTRIDGE_SUPER_SNAPSHOT:
        cartridge_config_changed(9);
        break;
      case CARTRIDGE_SUPER_SNAPSHOT_V5:
        cartridge_store_io1((ADDRESS)0xde00, 2);
        break;
      case CARTRIDGE_OCEAN:
      case CARTRIDGE_FUNPLAY:
        cartridge_config_changed(1);
        cartridge_store_io1((ADDRESS)0xde00, 0);
        break;
      case CARTRIDGE_IEEE488:
        cartridge_config_changed(0);
        /* FIXME: Insert interface init here.  */
        break;
	  case CARTRIDGE_EXPERT:
		/*
		 * Initialize nmi/reset trap functions.
		 */
		interrupt_set_nmi_trap_func(&maincpu_int_status, cartridge_ack_nmi_reset);
		interrupt_set_reset_trap_func(&maincpu_int_status, cartridge_ack_nmi_reset);

		ramconfig = (1 << 1);       /* Disable ~EXROM */
		cartridge_config_changed(ramconfig);
		break;
    }
}

void cartridge_attach(int type, BYTE *rawcart)
{
    mem_cartridge_type = type;
    roml_bank = romh_bank = 0;
    switch (type) {
      case CARTRIDGE_GENERIC_8KB:
      case CARTRIDGE_IEEE488:
      case CARTRIDGE_EPYX_FASTLOAD:
      case CARTRIDGE_REX:
        memcpy(roml_banks, rawcart, 0x2000);
        cartridge_config_changed(0);
        break;
      case CARTRIDGE_GENERIC_16KB:
      case CARTRIDGE_SIMONS_BASIC:
      case CARTRIDGE_WESTERMANN:
      case CARTRIDGE_FINAL_I:
        memcpy(roml_banks, rawcart, 0x2000);
        memcpy(romh_banks, &rawcart[0x2000], 0x2000);
        cartridge_config_changed(1);
        break;
      case CARTRIDGE_ACTION_REPLAY:
      case CARTRIDGE_ATOMIC_POWER:
        memcpy(roml_banks, rawcart, 0x8000);
        memcpy(romh_banks, rawcart, 0x8000);
        cartridge_config_changed(0);
        break;
      case CARTRIDGE_KCS_POWER:
        memcpy(roml_banks, rawcart, 0x2000);
        memcpy(romh_banks, &rawcart[0x2000], 0x2000);
        cartridge_config_changed(0);
        break;
      case CARTRIDGE_FINAL_III:
        memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
        memcpy(&romh_banks[0x0000], &rawcart[0x2000], 0x2000);
        memcpy(&roml_banks[0x2000], &rawcart[0x4000], 0x2000);
        memcpy(&romh_banks[0x2000], &rawcart[0x6000], 0x2000);
        memcpy(&roml_banks[0x4000], &rawcart[0x8000], 0x2000);
        memcpy(&romh_banks[0x4000], &rawcart[0xa000], 0x2000);
        memcpy(&roml_banks[0x6000], &rawcart[0xc000], 0x2000);
        memcpy(&romh_banks[0x6000], &rawcart[0xe000], 0x2000);
        cartridge_config_changed(1);
        break;
      case CARTRIDGE_SUPER_SNAPSHOT:
        memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
        memcpy(&romh_banks[0x0000], &rawcart[0x2000], 0x2000);
        memcpy(&roml_banks[0x2000], &rawcart[0x4000], 0x2000);
        memcpy(&romh_banks[0x2000], &rawcart[0x6000], 0x2000);
        cartridge_config_changed(9);
        break;
      case CARTRIDGE_SUPER_SNAPSHOT_V5:
        memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
        memcpy(&romh_banks[0x0000], &rawcart[0x2000], 0x2000);
        memcpy(&roml_banks[0x2000], &rawcart[0x4000], 0x2000);
        memcpy(&romh_banks[0x2000], &rawcart[0x6000], 0x2000);
        memcpy(&roml_banks[0x4000], &rawcart[0x8000], 0x2000);
        memcpy(&romh_banks[0x4000], &rawcart[0xa000], 0x2000);
        memcpy(&roml_banks[0x6000], &rawcart[0xc000], 0x2000);
        memcpy(&romh_banks[0x6000], &rawcart[0xe000], 0x2000);
        cartridge_store_io1((ADDRESS)0xde00, 2);
        break;
      case CARTRIDGE_OCEAN:
      case CARTRIDGE_FUNPLAY:
        memcpy(roml_banks, rawcart, 0x2000 * 16);
        memcpy(romh_banks, &rawcart[0x2000 * 16], 0x2000 * 16);
        break;
      case CARTRIDGE_ULTIMAX:
        memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
        memcpy(&romh_banks[0x0000], &rawcart[0x0000], 0x2000);
        cartridge_config_changed(3);
        break;
      case CARTRIDGE_SUPER_GAMES:
        memcpy(&roml_banks[0x0000], &rawcart[0x0000], 0x2000);
        memcpy(&romh_banks[0x0000], &rawcart[0x2000], 0x2000);
        memcpy(&roml_banks[0x2000], &rawcart[0x4000], 0x2000);
        memcpy(&romh_banks[0x2000], &rawcart[0x6000], 0x2000);
        memcpy(&roml_banks[0x4000], &rawcart[0x8000], 0x2000);
        memcpy(&romh_banks[0x4000], &rawcart[0xa000], 0x2000);
        memcpy(&roml_banks[0x6000], &rawcart[0xc000], 0x2000);
        memcpy(&romh_banks[0x6000], &rawcart[0xe000], 0x2000);
        cartridge_config_changed(0);
        break;
	  case CARTRIDGE_EXPERT:
		memcpy(export_ram0, rawcart, 0x2000);

		ramconfig = (1 << 1);       /* Disable ~EXROM */
		enable_trigger = 0;
		cartridge_config_changed(ramconfig);
		break;
      default:
        mem_cartridge_type = CARTRIDGE_NONE;
    }
    return;
}

void cartridge_detach(int type)
{
    switch (type) {
      case CARTRIDGE_IEEE488:
      /* FIXME: Insert interface removal here.  */
      break;
    }
    cartridge_config_changed(6);
    mem_cartridge_type = CARTRIDGE_NONE;
    return;
}

void cartridge_freeze(int type)
{
    if (type == CARTRIDGE_ACTION_REPLAY || type == CARTRIDGE_ATOMIC_POWER
        || type == CARTRIDGE_SUPER_SNAPSHOT
        || type == CARTRIDGE_SUPER_SNAPSHOT_V5)
        cartridge_config_changed(35);
    if (type == CARTRIDGE_KCS_POWER || type == CARTRIDGE_FINAL_III
        || type == CARTRIDGE_FINAL_I)
        cartridge_config_changed(3);
}

void cartridge_ack_nmi_reset(void)
{
	if (mem_cartridge_type == CARTRIDGE_EXPERT)
		{
		switch (cartmode)
			{
			case CARTRIDGE_MODE_PRG:
			case CARTRIDGE_MODE_OFF:
				break;

			case CARTRIDGE_MODE_ON:
				enable_trigger = 1;
				break;
			}
		}
}
