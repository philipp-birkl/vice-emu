/*
 * vicii-chip-model.h - Chip model definitions for the VIC-II emulation.
 *
 * Written by
 *  Daniel Kahlin <daniel@kahlin.net>
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

#ifndef VICE_VICII_CHIP_MODEL_H
#define VICE_VICII_CHIP_MODEL_H

extern void vicii_chip_model_init(void);





/*
 *
 * 28    Visible
 *
 * 27-25 Sprites
 *   000  None
 *   001  Check Sprite DMA
 *
 * 24-23 VcRc
 *   00  None
 *   01  UpdateVc
 *   10  UpdateRc
 *
 * 22-20 Border
 *   000 None
 *   100 Check border L0
 *   101 Check border L1
 *   110 Check border R0
 *   111 Check border R1
 *
 * 21-16 XPos/8
 *
 * 15    Visible
 *
 * 14    May FetchC
 *
 * 13-9 Phi1 Fetch
 *   00--- Idle
 *   01nnn Sprite Ptr + DMA0
 *   10nnn Sprite DMA1 + DMA2
 *   110-- Refresh
 *   111-- FetchG
 *
 * 8-0 Check BA flags
 *   8       Check fetch BA
 *   7-0     Check sprite 7-0 BA
 */


#define CHECK_BRD_M       0xe0000000
#define CHECK_BRD_L       0x80000000
#define CHECK_BRD_R       0x40000000
#define CHECK_BRD_CSEL    0x20000000

#define CHECK_SPR_DISP_M  0x01000000

#define VISIBLE_M         0x00400000

#define XPOS_M            0x003f0000
#define XPOS_B            16

#define PHI2_FETCH_C_M    0x00008000

#define PHI1_SPR_NUM_M    0x00007000
#define PHI1_SPR_NUM_B    12

#define PHI1_TYPE_M       0x00000e00
#define PHI1_TYPE_B       9
#define PHI1_IDLE         0x00000000
#define PHI1_REFRESH      0x00000200
#define PHI1_FETCH_G      0x00000400
#define PHI1_SPR_PTR      0x00000600
#define PHI1_SPR_DMA1     0x00000800

#define FETCH_BA_M        0x00000100
#define FETCH_BA_B        8
#define SPRITE_BA_MASK_M  0x000000ff
#define SPRITE_BA_MASK_B  0

static inline BYTE cycle_get_sprite_ba_mask(unsigned int flags)
{
    return (flags & SPRITE_BA_MASK_M) >> SPRITE_BA_MASK_B;
}

static inline int cycle_is_fetch_ba(unsigned int flags)
{
    return (flags & FETCH_BA_M) ? 1 : 0;
}

static inline int cycle_is_sprite_ptr_dma0(unsigned int flags)
{
    return (flags & PHI1_TYPE_M) == PHI1_SPR_PTR;
}

static inline int cycle_is_sprite_dma1_dma2(unsigned int flags)
{
    return (flags & PHI1_TYPE_M) == PHI1_SPR_DMA1;
}

static inline int cycle_get_sprite_num(unsigned int flags)
{
    return (flags & PHI1_SPR_NUM_M) >> PHI1_SPR_NUM_B;
}

static inline int cycle_is_refresh(unsigned int flags)
{
    return (flags & PHI1_TYPE_M) == PHI1_REFRESH;
}

static inline int cycle_is_fetch_g(unsigned int flags)
{
    return (flags & PHI1_TYPE_M) == PHI1_FETCH_G;
}

static inline int cycle_may_fetch_c(unsigned int flags)
{
    return (flags & PHI2_FETCH_C_M) ? 1 : 0;
}

static inline int cycle_is_visible(unsigned int flags)
{
    return (flags & VISIBLE_M) ? 1 : 0;
}

static inline int cycle_get_xpos(unsigned int flags)
{
    return ((flags & XPOS_M) >> XPOS_B) << 3;
}

static inline int cycle_is_check_spr_disp(unsigned int flags)
{
    return (flags & CHECK_SPR_DISP_M) ? 1 : 0;
}

static inline int cycle_is_check_border_l(unsigned int flags, int csel)
{
    if (flags & CHECK_BRD_L) {
        return (flags & CHECK_BRD_CSEL) ? csel : !csel;
    }
    return 0;
}

static inline int cycle_is_check_border_r(unsigned int flags, int csel)
{
    if (flags & CHECK_BRD_R) {
        return (flags & CHECK_BRD_CSEL) ? csel : !csel;
    }
    return 0;
}
#endif

