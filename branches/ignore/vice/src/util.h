/*
 * util.h - Miscellaneous utility functions.
 *
 * Written by
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

#ifndef _UTIL_H
#define _UTIL_H

#include "vice.h"

#include <stdarg.h>
#include <stdio.h>

#include "types.h"

#define UTIL_FILE_LOAD_RAW          0
#define UTIL_FILE_LOAD_SKIP_ADDRESS 1
#define UTIL_FILE_LOAD_FILL         2

extern char *util_concat(const char *s1, ...);
extern BYTE *util_bufcat(BYTE *buf, int *buf_size, size_t *max_buf_size,
                         const BYTE *src, int src_size);
extern void util_remove_spaces(char *s);
extern void util_add_extension(char **name, const char *extension);
extern char *util_add_extension_const(const char *filename,
                                      const char *extension);
extern char *util_get_extension(char *filename);

extern size_t util_file_length(FILE *fd);
extern int util_file_exists(const char *name);
extern int util_file_load(const char *name, BYTE *dest, size_t size,
                          unsigned int load_flag);
extern int util_file_save(const char *name, BYTE *src, int size);

extern int util_get_line(char *buf, int bufsize, FILE *f);
extern void util_fname_split(const char *path, char **directory_return,
                             char **name_return);

extern int util_string_to_long(const char *str, const char **endptr, int base,
                               long *result);
extern char *util_subst(const char *s, const char *string,
                        const char *replacement);
extern int util_string_set(char **str, const char *new_value);
extern int util_check_null_string(const char *string);

extern int util_dword_read(FILE *fd, DWORD *buf, size_t num);
extern int util_dword_write(FILE *fd, DWORD *buf, size_t num);
extern void util_dword_to_le_buf(BYTE *buf, DWORD data);

extern char *util_find_prev_line(const char *text, const char *pos);
extern char *util_find_next_line(const char *pos);

extern unsigned long util_crc32(const char *buffer, unsigned int len);
extern unsigned long util_crc32_file(const char *filename);

#if !defined HAVE_MEMMOVE
void *memmove(void *target, const void *source, unsigned int length);
#endif

#if !defined HAVE_ATEXIT
int atexit(void (*function)(void));
#endif

#if !defined HAVE_STRERROR
char *strerror(int errnum);
#endif

#if !defined HAVE_STRCASECMP
int strcasecmp(const char *s1, const char *s2);
#endif

#if !defined HAVE_STRNCASECMP
int strncasecmp(const char *s1, const char *s2, unsigned int n);
#endif

#endif /* UTILS_H */

