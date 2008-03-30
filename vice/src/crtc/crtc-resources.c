/*
 * crtc-resources.c - A line-based CRTC emulation (under construction).
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
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

#include "crtc-resources.h"
#include "crtc.h"
#include "crtctypes.h"
#include "raster-resources.h"
#include "resources.h"
#include "utils.h"
#ifdef USE_XF86_EXTENSIONS
#include "fullscreen.h"
#endif


crtc_resources_t crtc_resources;


static int set_palette_file_name(resource_value_t v, void *param)
{
    util_string_set(&crtc_resources.palette_file_name, (char *)v);
    if (crtc.initialized)
        return crtc_load_palette(crtc_resources.palette_file_name);

    return 0;
}

#ifdef CRTC_NEED_2X

static int set_double_size_enabled(resource_value_t v, void *param)
{
    crtc_resources.double_size_enabled = (int)v;

#ifdef USE_XF86_EXTENSIONS
    if (!fullscreen_is_enabled)
#endif
        crtc_resize();

    return 0;
}
#endif

#if defined CRTC_NEED_2X || defined __MSDOS__
static int set_double_scan_enabled(resource_value_t v, void *param)
{
    crtc_resources.double_scan_enabled = (int)v;
#ifdef USE_XF86_EXTENSIONS
    if (crtc.initialized && ! fullscreen_is_enabled)
#else
    if (crtc.initialized)
#endif
    {
        raster_enable_double_scan(&crtc.raster,
                                  crtc_resources.double_scan_enabled);
        crtc_resize();
    }
    return 0;
}
#endif

#ifdef USE_XF86_EXTENSIONS
static int set_fullscreen_double_size_enabled(resource_value_t v, void *param)
{
    crtc_resources.fullscreen_double_size_enabled = (int)v;
    if (fullscreen_is_enabled)
        crtc_resize();
    return 0;
}

static int set_fullscreen_double_scan_enabled(resource_value_t v, void *param)
{
    crtc_resources.fullscreen_double_scan_enabled = (int)v;
    if (crtc.initialized && fullscreen_is_enabled)
        raster_enable_double_scan(&crtc.raster,
                                  crtc_resources.double_scan_enabled);
    return 0;
}
#endif

static resource_t resources[] =
{
  { "CrtcPaletteFile", RES_STRING, (resource_value_t)"green",
    (resource_value_t *)&crtc_resources.palette_file_name,
    set_palette_file_name, NULL },
#ifdef CRTC_NEED_2X
  { "CrtcDoubleSize", RES_INTEGER, (resource_value_t)0,
    (resource_value_t *)&crtc_resources.double_size_enabled,
    set_double_size_enabled, NULL },
#ifdef USE_XF86_EXTENSIONS
  { "FullscreenDoubleSize", RES_INTEGER, (resource_value_t)0,
    (resource_value_t *)&crtc_resources.fullscreen_double_size_enabled,
    set_fullscreen_double_size_enabled, NULL },
  { "FullscreenDoubleScan", RES_INTEGER, (resource_value_t)0,
    (resource_value_t *)&crtc_resources.fullscreen_double_scan_enabled,
    set_fullscreen_double_scan_enabled, NULL },
#endif
#endif
#if defined CRTC_NEED_2X || defined __MSDOS__
  { "CrtcDoubleScan", RES_INTEGER, (resource_value_t)0,
    (resource_value_t *)&crtc_resources.double_scan_enabled,
    set_double_scan_enabled, NULL },
#endif
  { NULL }
};

int crtc_resources_init(void)
{
    if (raster_resources_chip_init("Crtc", &crtc.raster) < 0)
        return -1;

    return resources_register(resources);
}

