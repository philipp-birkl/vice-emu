/*
 * sid-snapshot.c - SID snapshot.
 *
 * Written by
 *  Teemu Rantanen <tvr@cs.hut.fi>
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Andreas Boose <viceteam@t-online.de>
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

#include "log.h"
#include "resources.h"
#include "sid-snapshot.h"
#include "sid.h"
#include "sound.h"
#include "snapshot.h"
#include "types.h"


static const char snap_module_name_simple[] = "SID";
#define SNAP_MAJOR_SIMPLE 1
#define SNAP_MINOR_SIMPLE 0

static int sid_snapshot_write_module_simple(snapshot_t *s)
{
    int sound, sid_engine;
    snapshot_module_t *m;

    m = snapshot_module_create(s, snap_module_name_simple, SNAP_MAJOR_SIMPLE,
                               SNAP_MINOR_SIMPLE);
    if (m == NULL)
        return -1;

    resources_get_value("Sound", (void *)&sound);
    if (SMW_B(m, (BYTE)sound) < 0) {
        snapshot_module_close(m);
        return -1;
    }

    if (sound) {
        resources_get_value("SidEngine", (void *)&sid_engine);
        if (SMW_B(m, (BYTE)sid_engine) < 0) {
            snapshot_module_close(m);
            return -1;
        }

        /* FIXME: Only data for first SID stored. */
        if (SMW_BA(m, sid_get_siddata(0), 32) < 0) {
            snapshot_module_close(m);
            return -1;
        }
    }

    snapshot_module_close(m);
    return 0;
}

static int sid_snapshot_read_module_simple(snapshot_t *s)
{
    BYTE major_version, minor_version;
    BYTE sound, sid_engine;
    snapshot_module_t *m;

    sound_close();

    m = snapshot_module_open(s, snap_module_name_simple,
                             &major_version, &minor_version);
    if (m == NULL)
        return -1;

    if (major_version > SNAP_MAJOR_SIMPLE
        || minor_version > SNAP_MINOR_SIMPLE) {
        log_error(LOG_DEFAULT,
                  "SID: Snapshot module version (%d.%d) newer than %d.%d.\n",
                  major_version, minor_version,
                  SNAP_MAJOR_SIMPLE, SNAP_MINOR_SIMPLE);
        return snapshot_module_close(m);
    }

    if (SMR_B(m, &sound) < 0) {
        snapshot_module_close(m);
        return -1;
    }

    resources_set_value("Sound", (resource_value_t)sound);

    if (sound) {
        if (SMR_B(m, &sid_engine) < 0) {
            snapshot_module_close(m);
            return -1;
        }
        resources_set_value("SidEngine", (resource_value_t)sid_engine);


        /* FIXME: Only data for first SID read. */
        if (SMR_BA(m, sid_get_siddata(0), 32) < 0) {
            snapshot_module_close(m);
            return -1;
        }

        sound_open();
    }

    return snapshot_module_close(m);
}

static const char snap_module_name_extended[] = "SIDEXTENDED";
#define SNAP_MAJOR_EXTENDED 1
#define SNAP_MINOR_EXTENDED 0

static int sid_snapshot_write_module_extended(snapshot_t *s)
{
    snapshot_module_t *m;
    sid_snapshot_state_t sid_state;
    int sound, sid_engine;

    resources_get_value("Sound", (void *)&sound);

    if (sound == 0)
        return 0;

    resources_get_value("SidEngine", (void *)&sid_engine);

    if (sid_engine != SID_ENGINE_FASTSID
#ifdef HAVE_RESID
        && sid_engine != SID_ENGINE_RESID
#endif
        )
        return 0;

    sid_state_read(0, &sid_state);

    m = snapshot_module_create(s, snap_module_name_extended,
                               SNAP_MAJOR_EXTENDED, SNAP_MINOR_EXTENDED);
    if (m == NULL)
        return -1;

    if (SMW_BA(m, sid_state.sid_register, 32) < 0
        || SMW_B(m, sid_state.bus_value) < 0
        || SMW_DW(m, sid_state.bus_value_ttl) < 0
        || SMW_DWA(m, sid_state.accumulator, 3) < 0
        || SMW_DWA(m, sid_state.shift_register, 3) < 0
        || SMW_WA(m, sid_state.rate_counter, 3) < 0
        || SMW_WA(m, sid_state.exponential_counter, 3) < 0
        || SMW_BA(m, sid_state.envelope_counter, 3) < 0
        || SMW_BA(m, sid_state.envelope_state, 3) < 0
        || SMW_BA(m, sid_state.hold_zero, 3) < 0) {
        snapshot_module_close(m);
        return -1;
    }

    snapshot_module_close(m);
    return 0;
}

static int sid_snapshot_read_module_extended(snapshot_t *s)
{
    BYTE major_version, minor_version;
    snapshot_module_t *m;
    sid_snapshot_state_t sid_state;
    int sound, sid_engine;

    resources_get_value("Sound", (void *)&sound);

    if (sound == 0)
        return 0;

    resources_get_value("SidEngine", (void *)&sid_engine);

    if (sid_engine != SID_ENGINE_FASTSID
#ifdef HAVE_RESID
        && sid_engine != SID_ENGINE_RESID
#endif
        )
        return 0;

    m = snapshot_module_open(s, snap_module_name_extended,
                             &major_version, &minor_version);
    if (m == NULL)
        return -1;

    if (major_version > SNAP_MAJOR_EXTENDED
        || minor_version > SNAP_MINOR_EXTENDED) {
        log_error(LOG_DEFAULT,
                  "SID: Snapshot module version (%d.%d) newer than %d.%d.\n",
                  major_version, minor_version,
                  SNAP_MAJOR_EXTENDED, SNAP_MINOR_EXTENDED);
        return snapshot_module_close(m);
    }

    if (SMR_BA(m, sid_state.sid_register, 32) < 0
        || SMR_B(m, &(sid_state.bus_value)) < 0
        || SMR_DW(m, &(sid_state.bus_value_ttl)) < 0
        || SMR_DWA(m, sid_state.accumulator, 3) < 0
        || SMR_DWA(m, sid_state.shift_register, 3) < 0
        || SMR_WA(m, sid_state.rate_counter, 3) < 0
        || SMR_WA(m, sid_state.exponential_counter, 3) < 0
        || SMR_BA(m, sid_state.envelope_counter, 3) < 0
        || SMR_BA(m, sid_state.envelope_state, 3) < 0
        || SMR_BA(m, sid_state.hold_zero, 3) < 0) {
        snapshot_module_close(m);
        return -1;
    }

    sid_state_write(0, &sid_state);

    return snapshot_module_close(m);
}

int sid_snapshot_write_module(snapshot_t *s)
{
    if (sid_snapshot_write_module_simple(s) < 0)
        return -1;

    if (sid_snapshot_write_module_extended(s) < 0)
        return -1;

    return 0;
}

int sid_snapshot_read_module(snapshot_t *s)
{
    if (sid_snapshot_read_module_simple(s) < 0)
        return -1;

    sid_snapshot_read_module_extended(s);

    return 0;
}

