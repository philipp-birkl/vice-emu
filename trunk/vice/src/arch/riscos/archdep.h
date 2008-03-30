/*
 * archdep.h - Architecture dependent defines.
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

#ifndef _ARCHDEP_H
#define _ARCHDEP_H

#include "archapi.h"
#include "posix.h"

/* Name of the resource file.  */
#define RESOURCE_FILE_NAME "VICERC"

/* Filesystem dependant operators.  */
#define FSDEVICE_DEFAULT_DIR   "@"
#define FSDEV_DIR_SEP_STR      "."
#define FSDEV_DIR_SEP_CHR      '.'
#define FSDEV_EXT_SEP_STR      "/"
#define FSDEV_EXT_SEP_CHR      '/'

/* Path separator.  */
#define FINDPATH_SEPARATOR_CHAR         ':'
#define FINDPATH_SEPARATOR_STRING       ":"

/* Modes for fopen().  */
#define MODE_READ              "r"
#define MODE_READ_TEXT         "r"
#define MODE_READ_WRITE        "r+"
#define MODE_WRITE             "w"
#define MODE_WRITE_TEXT        "w"
#define MODE_APPEND            "w+"
#define MODE_APPEND_READ_WRITE "a+"

/* Printer default devices.  */
#define PRINTER_DEFAULT_DEV1 "Printout"
#define PRINTER_DEFAULT_DEV2 "parallel:"
#define PRINTER_DEFAULT_DEV3 "serial:"

/* Video chip scaling.  */
#define ARCHDEP_VICII_DSIZE 1
#define ARCHDEP_VICII_DSCAN 1
#define ARCHDEP_VDC_DSIZE   1
#define ARCHDEP_VDC_DSCAN   1
#define ARCHDEP_VIC_DSIZE   1
#define ARCHDEP_VIC_DSCAN   1
#define ARCHDEP_CRTC_DSIZE  1
#define ARCHDEP_CRTC_DSCAN  1
#define ARCHDEP_TED_DSIZE   1
#define ARCHDEP_TED_DSCAN   1

/* Line editing */
extern char *readline(const char *prompt);
extern void add_history(const char *p);
extern const char *archdep_extract_dir_and_leaf(const char *path);

/* miscellaneous */
extern void archdep_closedown(void);
extern FILE *archdep_get_default_log_file(void);

#endif
