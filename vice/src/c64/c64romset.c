/*
 * c64romset.c
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

#include "c64romset.h"
#include "romset.h"


const char *machine_romset_resources_list[] = {
    "ChargenName",
    "KernalName",
    "BasicName",
    "DosName1541",
    "DosName1541ii",
    "DosName1570",
    "DosName1571",
    "DosName1581",
    "DosName2031",
    "DosName2040",
    "DosName3040",
    "DosName4040",
    "DosName1001",
    NULL
};


int machine_romset_load(char *filename)
{
    return romset_load(filename);
}

int machine_romset_save(char *filename)
{
    return romset_save(filename, machine_romset_resources_list);
}

char *machine_romset_file_list(const char *delim)
{
    return romset_file_list(machine_romset_resources_list, delim);
}

void machine_romset_init(void)
{
}

