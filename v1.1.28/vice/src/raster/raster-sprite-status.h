/*
 * raster-sprite-status.h - Sprite status handling for the raster emulation.
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

#ifndef _RASTER_SPRITE_STATUS_H
#define _RASTER_SPRITE_STATUS_H

#include "raster-sprite.h"
#include "types.h"

typedef void (*raster_sprite_status_draw_function_t) (PIXEL * line_ptr,
                                                      BYTE * gfx_msk_ptr);

struct _raster_sprite_status
  {
    unsigned int num_sprites;

    raster_sprite_t *sprites;

    raster_sprite_status_draw_function_t draw_function;

    /* Bit mask for the sprites that are activated.  */
    BYTE visible_msk;

    /* Bit mask for the sprites that have DMA enabled.  */
    BYTE dma_msk;

    /* Value for `dma_msk', after sprites have been drawn.  */
    BYTE new_dma_msk;

    unsigned int mc_sprite_color_1, mc_sprite_color_2;

    /* Place where the sprite memory pointers are. */
    BYTE *ptr_base;

    /* Sprite-sprite and sprite-background collisions detected on the current
       line. */
    BYTE sprite_sprite_collisions;
    BYTE sprite_background_collisions;

    /* Sprite data. */
    /* As we have to fetch sprite data for the next line while drawing the
       current one, we need two buffers.  Notice that we put this here
       (instead of putting it into the `sprite_t' struct) so that it is
       faster to swap between the two data pools.  */
    DWORD *sprite_data_1;
    DWORD *sprite_data_2;
    DWORD *sprite_data;
    DWORD *new_sprite_data;
  };
typedef struct _raster_sprite_status raster_sprite_status_t;



void raster_sprite_status_init (raster_sprite_status_t *s,
                                unsigned int num_sprites);
raster_sprite_status_t *raster_sprite_status_new (unsigned int num_sprites);
void raster_sprite_status_set_draw_function (raster_sprite_status_t *status,
                             raster_sprite_status_draw_function_t function);

#endif /* _RASTER_SPRITE_STATUS_H */
