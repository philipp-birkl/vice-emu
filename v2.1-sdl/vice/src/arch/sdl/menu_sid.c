/*
 * menu_sid.c - Implementation of the SID settings menu for the SDL UI.
 *
 * Written by
 *  Hannu Nuotio <hannu.nuotio@tut.fi>
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

#include <stdlib.h>

#include "lib.h"
#include "menu_common.h"
#include "menu_sid.h"
#include "resources.h"
#include "sid.h"
#include "uimenu.h"

UI_MENU_DEFINE_RADIO(SidModel)

static const ui_menu_entry_t sid_model_menu[] = {
    { "6581 (old)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581 },
    { "8580 (new)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580 },
    { "8580 + digi boost",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580D },
#ifdef HAVE_RESID_FP
    { "6581R3 4885 (very light)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_4885 },
    { "6581R3 0486S (light)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_0486S },
    { "6581R3 3984 (light avg)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_3984 },
    { "6581R4AR 3789 (avg)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R4AR_3789 },
    { "6581R3 4485 (dark)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_4485 },
    { "6581R4 1986S (very dark)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R4_1986S },
    { "8580 3691 (light)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_3691 },
    { "8580 3691 + digi boost",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_3691D },
    { "8580 1489 (dark)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_1489 },
    { "8580 1489 + digi boost",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_1489D },
#endif
    { NULL }
};

static const ui_menu_entry_t sid_dtv_model_menu[] = {
    { "DTVSID",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_DTVSID },
    { "6581 (old)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581 },
    { "8580 (new)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580 },
#ifdef HAVE_RESID_FP
    { "6581R3 4885 (very light)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_4885 },
    { "6581R3 0486S (light)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_0486S },
    { "6581R3 3984 (light avg)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_3984 },
    { "6581R4AR 3789 (avg)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R4AR_3789 },
    { "6581R3 4485 (dark)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R3_4485 },
    { "6581R4 1986S (very dark)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_6581R4_1986S },
    { "8580 3691 (light)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_3691 },
    { "8580 3691 + digi boost",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_3691D },
    { "8580 1489 (dark)",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_1489 },
    { "8580 1489 + digi boost",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidModel_callback,
      (ui_callback_data_t)SID_MODEL_8580R5_1489D },
#endif
    { NULL }
};

UI_MENU_DEFINE_RADIO(SidEngine)

static const ui_menu_entry_t sid_engine_menu[] = {
    { "Fast SID",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_FASTSID },
#ifdef HAVE_RESID
    { "ReSID",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_RESID },
#endif
#ifdef HAVE_RESID_FP
    { "ReSID-FP",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_RESID_FP },
#endif
#ifdef HAVE_CATWEASELMKIII
    { "Catweasel MKIII",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_CATWEASELMKIII },
#endif
#ifdef HAVE_PARSID
    { "ParSID Port 1",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_PARSID_PORT1 },
    { "ParSID Port 2",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_PARSID_PORT2 },
    { "ParSID Port 3",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidEngine_callback,
      (ui_callback_data_t)SID_ENGINE_PARSID_PORT3 },
#endif
    { NULL }
};

#ifdef HAVE_RESID
UI_MENU_DEFINE_RADIO(SidResidSampling)

static const ui_menu_entry_t sid_sampling_menu[] = {
    { "Fast",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidResidSampling_callback,
      (ui_callback_data_t)0 },
    { "Interpolating",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidResidSampling_callback,
      (ui_callback_data_t)1 },
    { "Resampling",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidResidSampling_callback,
      (ui_callback_data_t)2 },
    { "Fast Resampling",
      MENU_ENTRY_RESOURCE_RADIO,
      radio_SidResidSampling_callback,
      (ui_callback_data_t)3 },
    { NULL }
};

UI_MENU_DEFINE_INT(SidResidPassband)
#endif

UI_MENU_DEFINE_TOGGLE(SidFilters)
UI_MENU_DEFINE_TOGGLE(SidStereo)

static UI_MENU_CALLBACK(custom_SidStereoAddressStart_callback)
{
    static char buf[20];
    char *value = NULL;
    int previous, new_value;

    resources_get_int("SidStereoAddressStart", &previous);

    if (activated) {
        sprintf(buf, "0x%04x", previous);
        value = sdl_ui_text_input_dialog("Enter second SID base address", buf);
        if (value) {
            new_value = strtol(value, NULL, 0);
            if (new_value != previous) {
                resources_set_int("SidStereoAddressStart", new_value);
            }
            lib_free(value);
        }
    } else {
        sprintf(buf,"$%04x", previous);
        return buf;
    }
    return NULL;
}

const ui_menu_entry_t sid_c64_menu[] = {
    { "SID Model",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)sid_model_menu },
    { "SID Engine",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)sid_engine_menu },
    { "Second SID",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_SidStereo_callback,
      NULL },
    { "Second SID base address",
      MENU_ENTRY_DIALOG,
      custom_SidStereoAddressStart_callback,
      NULL },
    { "Emulate filters",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_SidFilters_callback,
      NULL },
#ifdef HAVE_RESID
    { "reSID sampling method",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)sid_sampling_menu },
    { "reSID resampling passband",
      MENU_ENTRY_RESOURCE_INT,
      int_SidResidPassband_callback,
      (ui_callback_data_t)"Enter passband in percentage of total bandwidth (0 - 90, lower is faster, higher is better)" },
#endif
    { NULL }
};

const ui_menu_entry_t sid_dtv_menu[] = {
    { "SID Model",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)sid_dtv_model_menu },
    { "SID Engine",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)sid_engine_menu },
    { "Emulate filters",
      MENU_ENTRY_RESOURCE_TOGGLE,
      toggle_SidFilters_callback,
      NULL },
#ifdef HAVE_RESID
    { "reSID sampling method",
      MENU_ENTRY_SUBMENU,
      submenu_radio_callback,
      (ui_callback_data_t)sid_sampling_menu },
    { "reSID resampling passband",
      MENU_ENTRY_RESOURCE_INT,
      int_SidResidPassband_callback,
      (ui_callback_data_t)"Enter passband in percentage of total bandwidth (0 - 90, lower is faster, higher is better)" },
#endif
    { NULL }
};

