/*
 * mpegdrv.h - Movie driver using FFMPEG library and screenshot API.
 *
 * Written by
 *  Andreas Matthies <andreas.matthies@gmx.net>
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

#ifndef _MPEGDRV_H
#define _MPEGDRV_H

#include "screenshot.h"

typedef struct mpegdrv_audio_in_s {
    SWORD *buffer;
    int buffersamples;
    int used;
} mpegdrv_audio_in_t;

extern void gfxoutput_init_mpeg(void);
extern void mpegdrv_init_audio(int speed, int channels, 
                               mpegdrv_audio_in_t** audio_in);
extern void mpegdrv_encode_audio(mpegdrv_audio_in_t *audio_in);

#endif
