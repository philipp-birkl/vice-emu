/*
 * uihelp.h - RISC OS interactive help data structures.
 *
 * Written by
 *  Andreas Dehmel <dehmel@forwiss.tu-muenchen.de>
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



#ifndef _UIHELP_RO_H
#define _UIHELP_RO_H

#include "wimp.h"
#include "uiconfig.h"
#include "uisharedef.h"

typedef struct help_icon_s {
  int icon;
  const char *sym;
  char *msg;
} help_icon_t;


extern help_icon_t *Help_ConfigWindows[CONF_WIN_NUMBER];

extern void ui_translate_icon_help_msgs(const wimp_msg_desc *msg, help_icon_t *hi);
extern void ui_translate_help_messages(const wimp_msg_desc *msg);
extern const char *ui_get_help_for_window_icon(int handle, int icon);

#endif
