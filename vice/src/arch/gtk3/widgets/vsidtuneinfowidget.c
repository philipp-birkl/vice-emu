/** \file   vsidmainwidget.c
 * \brief   GTK3 tune info widget for VSID
 *
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

/*
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

#include <stdlib.h>
#include <gtk/gtk.h>

#include "vice_gtk3.h"
#include "debug.h"
#include "machine.h"
#include "lib.h"
#include "log.h"
#include "util.h"

#include "vsidtuneinfowidget.h"

static int tune_count;
static int tune_current;
static int tune_default;
static const char *irq_source = "";
static int model_id;
static int sync_id;
static unsigned int runtime_sec;
static const char *driver_info;

static GtkWidget *tune_info_grid;
static GtkWidget *name_widget;
static GtkWidget *author_widget;
static GtkWidget *copyright_widget;
static GtkWidget *tune_num_widget;
static GtkWidget *model_widget;
static GtkWidget *irq_widget;
static GtkWidget *sync_widget;
static GtkWidget *runtime_widget;
static GtkWidget *driver_info_widget;


static GtkWidget *create_left_aligned_label(const char *text)
{
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}


static GtkWidget *create_readonly_entry(void)
{
#if 0
    GtkWidget *entry = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
    gtk_widget_set_can_focus(entry, FALSE);
    /* TODO: use CSS to make the entry appear 'flat' and perhaps remove the
     *       frame */

    return entry;
#endif
    GtkWidget *label = gtk_label_new(NULL);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}


static GtkWidget *create_tune_num_widget(void)
{
    GtkWidget *label;
    char *text;

    text = lib_msprintf("%d of %d (Default: %d)",
            tune_current, tune_count, tune_default);
    label = gtk_label_new(text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    lib_free(text);
    return label;
}


/** \brief  Update tune number widget
 *
 * Updates the 'X of Y (default: Z)' widget
 */
static void update_tune_num_widget(void)
{
    char *text;

    text = lib_msprintf("%d of %d (Default: %d)",
            tune_current, tune_count, tune_default);
    gtk_label_set_text(GTK_LABEL(tune_num_widget), text);
    lib_free(text);
}


/** \brief  Create IRQ widget
 */
static GtkWidget *create_irq_widget(void)
{
    GtkWidget *label;

    label = gtk_label_new(irq_source);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}


/** \brief  Update IRQ widget
 */
static void update_irq_widget(void)
{
    gtk_label_set_text(GTK_LABEL(irq_widget), irq_source);
}


/** \brief  Create SID model widget
 */
static GtkWidget *create_model_widget(void)
{
    GtkWidget *label;

    if (model_id == 0) {
        label = gtk_label_new("6581");
    } else {
        label = gtk_label_new("8580");
    }
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}


/** \brief  Update SID model widget
 */
static void update_model_widget(void)
{
    if (model_id == 0) {
        gtk_label_set_text(GTK_LABEL(model_widget), "MOS 6581");
    } else {
        gtk_label_set_text(GTK_LABEL(model_widget), "MOS 8580");
    }
}


/** \brief  Create run time widget
 *
 * Creates a widget which displays hours, minutes and seconds
 */
static GtkWidget *create_runtime_widget(void)
{
    GtkWidget *label = gtk_label_new("0:00:00");
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}


/** \brief  Update the run time widget
 */
static void update_runtime_widget(void)
{
    char buffer[256];
    unsigned int s;
    unsigned int m;
    unsigned int h;

    s = runtime_sec % 60;
    m = runtime_sec / 60;
    h = runtime_sec / 60 / 60;

    /* don't use lib_msprintf() here, this function gets called a lot and
     * malloc() isn't fast */
    g_snprintf(buffer, 256, "%u:%02u:%02u", h, m, s);
    gtk_label_set_text(GTK_LABEL(runtime_widget), buffer);
}


/** \brief  Create sync widget
 */
static GtkWidget *create_sync_widget(void)
{
    GtkWidget *label;

    if (sync_id == 1) {
        label = gtk_label_new("PAL (50Hz)");
    } else {
        label = gtk_label_new("NTSC (60Hz)");
    }
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    return label;
}


/** \brief  Update sync widget
 */
static void update_sync_widget(void)
{
    if (sync_id == 1) {
        gtk_label_set_text(GTK_LABEL(sync_widget), "PAL (50Hz)");
    } else {
        gtk_label_set_text(GTK_LABEL(sync_widget), "NTSC (60Hz)");
    }
}


/** \brief  Create driver information widget
 */
static GtkWidget *create_driver_info_widget(void)
{
    GtkWidget *label = gtk_label_new(NULL);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    return label;
}


/** \brief  Set new driver info
 *
 * The info gotten from vsid is a string with parameters separated by commas.
 * This function replaces the commas (and the following space) with new lines
 * so the information is displayed slighly better looking.
 *
 * XXX: Still looks ugly though, perhaps parse the string to retrieve the
 *      various parameters and use separate widgets to display them to get a
 *      proper layout.
 */
static void update_driver_info_widget(void)
{
    char *drv;

    drv = util_subst(driver_info, ", ", "\n");
    gtk_label_set_text(GTK_LABEL(driver_info_widget), drv);
    lib_free(drv);
}


/** \brief  Create widget to show tune information
 *
 * \return  GtkGrid
 */
GtkWidget *vsid_tune_info_widget_create(void)
{
    GtkWidget *grid;
    GtkWidget *label;

    grid = vice_gtk3_grid_new_spaced(VICE_GTK3_DEFAULT, VICE_GTK3_DEFAULT);

    /* title */
    label = create_left_aligned_label("Name:");
    name_widget = create_readonly_entry();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_widget, 1, 0, 1, 1);

    /* author */
    label = create_left_aligned_label("Author:");
    author_widget = create_readonly_entry();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), author_widget, 1, 1, 1, 1);

    /* copyright */
    label = create_left_aligned_label("Copyright:");
    copyright_widget = create_readonly_entry();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), copyright_widget, 1, 2, 1, 1);

    /* tune number (x of x) */
    label = create_left_aligned_label("Tune:");
    tune_num_widget = create_tune_num_widget();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), tune_num_widget, 1, 3, 1, 1);

    /* model widget */
    label = create_left_aligned_label("Model:");
    model_widget = create_model_widget();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), model_widget, 1, 4, 1, 1);

    /* IRQ widget */
    label = create_left_aligned_label("IRQ:");
    irq_widget = create_irq_widget();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), irq_widget, 1, 5, 1, 1);

    /* sync widget */
    label = create_left_aligned_label("Synchronization:");
    sync_widget = create_sync_widget();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), sync_widget, 1, 6, 1, 1);

    /* runtime widget */
    label = create_left_aligned_label("Run time:");
    runtime_widget = create_runtime_widget();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 7, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), runtime_widget, 1, 7, 1, 1);

    /* driver info */
    label = create_left_aligned_label("Driver info:");
    gtk_widget_set_valign(label, GTK_ALIGN_START);
    driver_info_widget = create_driver_info_widget();
    gtk_grid_attach(GTK_GRID(grid), label, 0, 8, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), driver_info_widget, 1,8, 1, 1);

    gtk_widget_show_all(grid);
    tune_info_grid = grid;
    return grid;
}


/** \brief  Set tune \a name
 *
 * \param[in]   name    tune name
 */
void vsid_tune_info_widget_set_name(const char *name)
{
    gtk_label_set_text(GTK_LABEL(name_widget), name);
}


/** \brief  Set author
 *
 * \param[in]   name    author name
 */
void vsid_tune_info_widget_set_author(const char *name)
{
    gtk_label_set_text(GTK_LABEL(author_widget), name);
}


/** \brief  Set copyright info string
 *
 * \param[in]   name    copyright string
 */
void vsid_tune_info_widget_set_copyright(const char *name)
{
    gtk_label_set_text(GTK_LABEL(copyright_widget), name);
}


/** \brief  Set number of tunes
 *
 * \param[in]   n   tune count
 */
void vsid_tune_info_widget_set_tune_count(int num)
{
    tune_count = num;
    update_tune_num_widget();
}


/** \brief  Set default tune
 *
 * \param[in]   n   tune number
 */
void vsid_tune_info_widget_set_tune_default(int num)
{
    tune_default = num;
    update_tune_num_widget();
}


/** \brief  Set current tune
 *
 * \param[in]   n   tune number
 */
void vsid_tune_info_widget_set_tune_current(int num)
{
    tune_current = num;
    update_tune_num_widget();
}


/** \brief  Set SID model
 *
 * \param[in]   model   model ID (0 = 6581, 1 = 8580)
 */
void vsid_tune_info_widget_set_model(int model)
{
    model_id = model;
    update_model_widget();
}


/** \brief  Set sync factor
 *
 * \param[in]   sync    sync factor (0 = 60Hz/NTSC, 1 = 50Hz/PAL)
 */
void vsid_tune_info_widget_set_sync(int sync)
{
    sync_id = sync;
    update_sync_widget();
}


/** \brief  Set IRQ source
 *
 * \param[in]   irq irq source string
 */
void vsid_tune_info_widget_set_irq(const char *irq)
{
    irq_source = irq;
    update_irq_widget();
}


/** \brief  Set current run time
 *
 * \param[in]   sec run time in seconds
 */
void vsid_tune_info_widget_set_time(unsigned int sec)
{
    runtime_sec = sec;
    update_runtime_widget();
}


/** \brief  Set driver information
 *
 * \param[in]   text    driver information
 */
void vsid_tune_info_widget_set_driver(const char *text)
{
    driver_info = text;
    update_driver_info_widget();
}
