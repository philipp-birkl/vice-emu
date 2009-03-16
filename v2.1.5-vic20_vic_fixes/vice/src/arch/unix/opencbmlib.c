/*
 * opencbmlib.c - Interface to access the opencbm library.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Christian Vogelgsang <chris@vogelgsang.org>
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

#ifdef HAVE_OPENCBM

#include "log.h"
#include "opencbmlib.h"
#include "dlfcn.h"

#ifdef MACOSX_SUPPORT
#define OPENCBM_LIBNAME "/opt/opencbm/lib/libopencbm.dylib"
#else
#define OPENCBM_LIBNAME "libopencbm.so"
#endif

static void *opencbmlib_handle = NULL;

#define GET_PROC_ADDRESS_AND_TEST(_name_) \
    opencbmlib->p_##_name_ = (_name_##_t)dlsym(opencbmlib_handle, #_name_); \
    if (opencbmlib->p_##_name_ == NULL) {                                   \
        log_debug("dlsym " #_name_ " failed!");                             \
    } else {                                                                \
        log_debug("dlsym " #_name_ " success: %p",opencbmlib->p_##_name_);  \
    }

static int opencbmlib_load_library(opencbmlib_t *opencbmlib)
{
    if(opencbmlib_handle == NULL) {
        opencbmlib_handle = dlopen(OPENCBM_LIBNAME,RTLD_LAZY|RTLD_LOCAL);
        
        if (opencbmlib_handle == NULL) {
            log_debug("loading shared library '" OPENCBM_LIBNAME "' failed!");
            return -1;
        }
        
        GET_PROC_ADDRESS_AND_TEST(cbm_driver_open);
        GET_PROC_ADDRESS_AND_TEST(cbm_driver_close);
        GET_PROC_ADDRESS_AND_TEST(cbm_get_driver_name);
        GET_PROC_ADDRESS_AND_TEST(cbm_listen);
        GET_PROC_ADDRESS_AND_TEST(cbm_talk);
        GET_PROC_ADDRESS_AND_TEST(cbm_open);
        GET_PROC_ADDRESS_AND_TEST(cbm_close);
        GET_PROC_ADDRESS_AND_TEST(cbm_raw_read);
        GET_PROC_ADDRESS_AND_TEST(cbm_raw_write);
        GET_PROC_ADDRESS_AND_TEST(cbm_unlisten);
        GET_PROC_ADDRESS_AND_TEST(cbm_untalk);
        GET_PROC_ADDRESS_AND_TEST(cbm_get_eoi);
        GET_PROC_ADDRESS_AND_TEST(cbm_reset);
        
        log_debug("loaded shared library '" OPENCBM_LIBNAME "'");
    }

    return 0;
}

static void opencbmlib_free_library(void)
{
    if (opencbmlib_handle != NULL) {
        if (dlclose(opencbmlib_handle) != 0) {
            log_debug("closing shared library '" OPENCBM_LIBNAME "' failed!");
        }
    }

    opencbmlib_handle = NULL;
}

int opencbmlib_open(opencbmlib_t *opencbmlib)
{
    return opencbmlib_load_library(opencbmlib);
}

void opencbmlib_close(void)
{
    opencbmlib_free_library();
}

unsigned int opencbmlib_is_available(void)
{
    if (opencbmlib_handle != NULL)
        return 1;

    return 0;
}

#endif

