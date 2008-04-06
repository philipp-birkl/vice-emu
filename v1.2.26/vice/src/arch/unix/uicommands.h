/*
 * uicommands.h - Implementation of common UI commands.
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

#ifndef _UI_COMMANDS_H
#define _UI_COMMANDS_H

#include "uimenu.h"

extern ui_menu_entry_t ui_directory_commands_menu[];
extern ui_menu_entry_t ui_disk_commands_menu[];
extern ui_menu_entry_t ui_exit_commands_menu[];
extern ui_menu_entry_t ui_help_commands_menu[];
extern ui_menu_entry_t ui_run_commands_menu[];
extern ui_menu_entry_t ui_smart_attach_commands_menu[];
extern ui_menu_entry_t ui_snapshot_commands_menu[];
extern ui_menu_entry_t ui_tape_commands_menu[];
extern ui_menu_entry_t ui_tool_commands_menu[];

#endif

