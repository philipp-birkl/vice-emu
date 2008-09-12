/*
 * uic128.c - Implementation of the C128-specific part of the UI.
 *
 * Written by
 *  Hannu Nuotio <hannu.nuotio@tut.fi>
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
#include <stdlib.h>

#include "debug.h"
#include "c128mem.h"
#include "menu_common.h"
#include "menu_reset.h"
#include "menu_speed.h"
#include "ui.h"
#include "uimenu.h"

/* the following is only kept as an example for now */
#if 0
UI_MENU_DEFINE_RADIO(SidModel)

static ui_menu_entry_t sid_model_menu[] = {
    { "6581 (old)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)0 },
    { "8580 (new)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)1 },
    { "8580 + digiboost",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)2 },
    { "DTVSID",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)4 },
    { NULL }
};

UI_MENU_DEFINE_TOGGLE(VICIIDoubleSize)

static ui_menu_entry_t x128_main_menu[] = {
    { "Attach disk",
      MENU_ENTRY_SUBMENU,
      NULL, /* disk_attach_dialog */
      NULL },
    { "Doublesize",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_VICIIDoubleSize_callback,
      NULL },
    SDL_MENU_ITEM_SEPARATOR,
    { "SID model",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)sid_model_menu },
    { "Quit",
      MENU_ENTRY_OTHER,
      quit_callback,
      NULL },
    { NULL }
};
#endif

/* temporary empty drive menu, this one will be moved out to menu_drive.c */
static ui_menu_entry_t drive_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty tape menu, this one will be moved out to menu_tape.c */
static ui_menu_entry_t tape_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty c128/c64 cart menu, this one will be moved out to menu_c64_c128_cart.c */
static ui_menu_entry_t c64_c128_cart_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty c128 hardware menu, this one will be moved out to menu_c128hw.c */
static ui_menu_entry_t c128_hardware_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty c128 rom menu, this one will be moved out to menu_c128rom.c */
static ui_menu_entry_t c128_rom_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty c128 video menu, this one will be moved out to menu_c128video.c */
static ui_menu_entry_t c128_video_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty sound menu, this one will be moved out to menu_sound.c */
static ui_menu_entry_t sound_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

/* temporary empty snapshot menu, this one will be moved out to menu_snapshot.c */
static ui_menu_entry_t snapshot_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

#ifdef DEBUG
/* temporary empty debug menu, this one will be moved out to menu_debug.c */
static ui_menu_entry_t debug_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};
#endif

/* temporary empty help menu, this one will be moved out to menu_help.c */
static ui_menu_entry_t help_menu[] = {
    SDL_MENU_ITEM_SEPARATOR,
    { NULL }
};

static const ui_menu_entry_t x128_main_menu[] = {
    { "Autostart image",
      MENU_ENTRY_DIALOG,
      autostart_callback,
      NULL },
    { "Drive",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)drive_menu },
    { "Tape",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)tape_menu },
    { "Cartridge",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c64_c128_cart_menu },
    { "Machine settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c128_hardware_menu },
    { "ROM settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c128_rom_menu },
    { "Video settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c128_video_menu },
    { "Sound settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)sound_menu },
    { "Snapshot",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)snapshot_menu },
    { "Speed settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)speed_menu },
    { "Reset",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)reset_menu },
    { "Pause",
      MENU_ENTRY_OTHER,
      pause_callback,
      NULL },
    { "Monitor",
      MENU_ENTRY_OTHER,
      monitor_callback,
      NULL },
#ifdef DEBUG
    { "Debug",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)debug_menu },
#endif
    { "Help",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)help_menu },
    { "Quit emulator",
      MENU_ENTRY_OTHER,
      quit_callback,
      NULL },
    { NULL }
};

int c128ui_init(void)
{
fprintf(stderr,"%s\n",__func__);

    sdl_ui_set_vcachename("VICIIVideoCache");
    sdl_ui_set_main_menu(x128_main_menu);
    sdl_ui_set_menu_font(mem_chargen_rom + 0x800, 8, 8);
    sdl_ui_set_menu_colors(1, 0);
    sdl_ui_set_menu_borders(0, 0);
    sdl_ui_set_double_x(0);
    return 0;
}

void c128ui_shutdown(void)
{
fprintf(stderr,"%s\n",__func__);
}

