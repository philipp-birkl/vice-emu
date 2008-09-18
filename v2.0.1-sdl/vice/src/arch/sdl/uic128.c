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
#include "menu_help.h"
#include "menu_reset.h"
#include "menu_screenshot.h"
#include "menu_settings.h"
#include "menu_sound.h"
#include "menu_speed.h"
#include "menu_video_c128.h"
#include "ui.h"
#include "uimenu.h"

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

static const ui_menu_entry_t x128_main_menu[] = {
    { "Autostart image",
      MENU_ENTRY_DIALOG,
      autostart_callback,
      NULL },
    { "Drive (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)drive_menu },
    { "Tape (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)tape_menu },
    { "Cartridge (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c64_c128_cart_menu },
    { "Machine settings (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c128_hardware_menu },
    { "ROM settings (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c128_rom_menu },
    { "Video settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)c128_video_menu },
    { "Sound output settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)sound_output_menu },
    { "Sound record settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)sound_record_menu },
    { "Snapshot (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)snapshot_menu },
    { "Screenshot (to be fixed)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)screenshot_menu },
    { "Speed settings",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)speed_menu },
    { "Reset",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)reset_menu },
    { "Pause (todo)",
      MENU_ENTRY_OTHER,
      pause_callback,
      NULL },
    { "Monitor (todo)",
      MENU_ENTRY_OTHER,
      monitor_callback,
      NULL },
#ifdef DEBUG
    { "Debug (todo)",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)debug_menu },
#endif
    { "Help",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)help_menu },
    { "Settings management",
      MENU_ENTRY_SUBMENU,
      NULL,
      (ui_callback_data_t)settings_manager_menu },
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

