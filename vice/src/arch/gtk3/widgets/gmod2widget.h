/** \file   src/arch/gtk3/widgets/gmod2widget.h
 * \brief   Widget to control GMod2 resources - header
 *
 * Written by
 *  Bas Wassink <b.wassink@ziggo.nl>
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

#ifndef VICE_GMOD2WIDGET_H
#define VICE_GMOD2WIDGET_H

#include "vice.h"
#include <gtk/gtk.h>

GtkWidget * gmod2_widget_create(GtkWidget *parent);
void        gmod2_widget_set_save_handler(int (*func)(int, const char *));
void        gmod2_widget_set_flush_handler(int (*func)(int));

#endif
