/*
 * machine-bus.c - Generic interface to IO bus functions.
 *
 * Written by
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

#include "lib.h"
#include "log.h"
#include "machine-bus.h"
#include "serial.h"
#include "types.h"


/* Call this if device is not attached: -128 == device not present.  */
static int fn(void)
{
    return 0x80;
}

void machine_bus_init(void)
{
    unsigned int i;

    for (i = 0; i < SERIAL_MAXDEVICES; i++) {
        serial_t *p;

        p = serial_device_get(i);

        p->inuse = 0;
        p->getf = (int (*)(struct vdrive_s *, BYTE *, unsigned int))fn;
        p->putf = (int (*)(struct vdrive_s *, BYTE, unsigned int))fn;
        p->openf = (int (*)(struct vdrive_s *, const BYTE *, unsigned int,
                   unsigned int))fn;
        p->closef = (int (*)(struct vdrive_s *, unsigned int))fn;
        p->flushf = (void (*)(struct vdrive_s *, unsigned int))NULL;
    }

    machine_bus_init_machine();
}

int machine_bus_device_attach(unsigned int unit, const char *name,
                              int (*getf)(struct vdrive_s *, BYTE *,
                              unsigned int),
                              int (*putf)(struct vdrive_s *, BYTE,
                              unsigned int),
                              int (*openf)(struct vdrive_s *, const BYTE *,
                              unsigned int, unsigned int),
                              int (*closef)(struct vdrive_s *, unsigned int),
                              void (*flushf)(struct vdrive_s *, unsigned int))
{
    serial_t *p;
    int i;

    if (unit >= SERIAL_MAXDEVICES)
        return 1;

    p = serial_device_get(unit);

    if (p->inuse != 0)
        machine_bus_device_detach(unit);

    p->getf = getf;
    p->putf = putf;
    p->openf = openf;
    p->closef = closef;
    p->flushf = flushf;

    p->inuse = 1;

    if (p->name != NULL)
        lib_free(p->name);

    p->name = lib_stralloc(name);

    for (i = 0; i < 16; i++) {
        p->nextok[i] = 0;
        p->isopen[i] = 0;
    }

    return 0;
}

/* Detach and kill serial devices.  */
int machine_bus_device_detach(unsigned int unit)
{
    serial_t *p;

    if (unit >= SERIAL_MAXDEVICES) {
        log_error(LOG_DEFAULT, "Illegal device number %d.", unit);
        return -1;
    }

    p = serial_device_get(unit);

    if (p != NULL && p->inuse != 0) {
        p->inuse = 0;
        lib_free(p->name);
        p->name = NULL;
    }

    return 0;
}

