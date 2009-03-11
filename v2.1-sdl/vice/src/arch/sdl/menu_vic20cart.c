/*
 * menu_vic20cart.c - Implementation of the vic20 cartridge settings menu for the SDL UI.
 *
 * Written by
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
#include <string.h>

#include "cartridge.h"
#include "keyboard.h"
#include "lib.h"
#include "menu_common.h"
#include "menu_vic20cart.h"
#include "resources.h"
#include "ui.h"
#include "uifilereq.h"
#include "uimenu.h"

static UI_MENU_CALLBACK(attach_cart_callback)
{
    char *title;
    char *name = NULL;

    if (activated) {
        switch ((int)(long)param) {
            case CARTRIDGE_VIC20_16KB_2000:
            case CARTRIDGE_VIC20_16KB_4000:
            case CARTRIDGE_VIC20_16KB_6000:
                title = "Select 4/8/16KB image";
                break;
            case CARTRIDGE_VIC20_8KB_A000:
                title = "Select 4/8KB image";
                break;
            case CARTRIDGE_VIC20_4KB_B000:
            default:
                title = "Select 4KB image";
                break;
        }
        name = sdl_ui_file_selection_dialog(title, FILEREQ_MODE_CHOOSE_FILE);
        if (name != NULL) {
            if (cartridge_attach_image((int)(long)param, name) < 0) {
                ui_error("Cannot load cartridge image.");
            }
            lib_free(name);
        }
    }
    return NULL;
}

static UI_MENU_CALLBACK(detach_cart_callback)
{
    if (activated) {
        cartridge_detach_image();
    }
    return NULL;
}

static UI_MENU_CALLBACK(set_cart_default_callback)
{
    if (activated) {
        cartridge_set_default();
    }
    return NULL;
}

const ui_menu_entry_t vic20cart_menu[] = {
    { "Attach 4/8/16KB image at $2000",
      MENU_ENTRY_DIALOG,
      attach_cart_callback,
      (ui_callback_data_t)CARTRIDGE_VIC20_16KB_2000 },
    { "Attach 4/8/16KB image at $4000",
      MENU_ENTRY_DIALOG,
      attach_cart_callback,
      (ui_callback_data_t)CARTRIDGE_VIC20_16KB_4000 },
    { "Attach 4/8/16KB image at $6000",
      MENU_ENTRY_DIALOG,
      attach_cart_callback,
      (ui_callback_data_t)CARTRIDGE_VIC20_16KB_6000 },
    { "Attach 4/8KB image at $A000",
      MENU_ENTRY_DIALOG,
      attach_cart_callback,
      (ui_callback_data_t)CARTRIDGE_VIC20_8KB_A000 },
    { "Attach 4KB image at $B000",
      MENU_ENTRY_DIALOG,
      attach_cart_callback,
      (ui_callback_data_t)CARTRIDGE_VIC20_4KB_B000 },
    SDL_MENU_ITEM_SEPARATOR,
    { "Detach cartridge image",
      MENU_ENTRY_OTHER,
      detach_cart_callback,
      NULL },
    { "Set current cartridge as default",
      MENU_ENTRY_OTHER,
      set_cart_default_callback,
      NULL },
    { NULL }
};
