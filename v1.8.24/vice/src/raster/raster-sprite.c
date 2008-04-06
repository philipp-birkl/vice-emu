/*
 * raster-sprite.c - Sprite handling.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
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

#include "raster-sprite.h"
#include "utils.h"


void raster_sprite_init(raster_sprite_t *s)
{
    s->x = 0;
    s->y = 0;
    s->x_expanded = 0;
    s->y_expanded = 0;
    s->multicolor = 0;
    s->in_background = 0;
    s->color = 0;
    s->memptr = 0;
    s->memptr_inc = 0;
    s->exp_flag = 0;
    s->dma_flag = 0;
}

raster_sprite_t *raster_sprite_new(void)
{
    raster_sprite_t *new_sprite;

    new_sprite = (raster_sprite_t *)xmalloc(sizeof(raster_sprite_t));
    raster_sprite_init(new_sprite);

    return new_sprite;
}

