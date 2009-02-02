//  ---------------------------------------------------------------------------
//  This file is part of reSID, a MOS6581 SID emulator engine.
//  Copyright (C) 2004  Dag Lem <resid@nimrod.no>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  ---------------------------------------------------------------------------

#include "wave.h"

reg8 WaveformGeneratorFP::wave6581__ST[] =
{
/* 0x000: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x008: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x010: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x018: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x020: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x028: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x030: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x038: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x040: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x048: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x050: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x058: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x060: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x068: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x070: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x078: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x080: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x088: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x090: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x098: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x0c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x0f8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0x100: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x108: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x110: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x118: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x120: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x128: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x130: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x138: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x140: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x148: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x150: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x158: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x160: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x168: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x170: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x178: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x180: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x188: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x190: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x198: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x1c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x1f8: */  0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f,
/* 0x200: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x208: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x210: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x218: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x220: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x228: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x230: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x238: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x240: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x248: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x250: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x258: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x260: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x268: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x270: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x278: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x280: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x288: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x290: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x298: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x2c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x2f8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0x300: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x308: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x310: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x318: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x320: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x328: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x330: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x338: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x340: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x348: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x350: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x358: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x360: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x368: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x370: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x378: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x380: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x388: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x390: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x398: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x3c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x3f0: */  0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
/* 0x3f8: */  0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x3f, 0x3f,
/* 0x400: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x408: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x410: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x418: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x420: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x428: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x430: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x438: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x440: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x448: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x450: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x458: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x460: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x468: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x470: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x478: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x480: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x488: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x490: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x498: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x4c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x4f8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0x500: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x508: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x510: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x518: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x520: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x528: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x530: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x538: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x540: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x548: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x550: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x558: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x560: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x568: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x570: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x578: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x580: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x588: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x590: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x598: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x5c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x5f8: */  0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x1f,
/* 0x600: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x608: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x610: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x618: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x620: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x628: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x630: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x638: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x640: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x648: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x650: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x658: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x660: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x668: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x670: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x678: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x680: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x688: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x690: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x698: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x6c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x6f8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0x700: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x708: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x710: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x718: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x720: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x728: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x730: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x738: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x740: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x748: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x750: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x758: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x760: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x768: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x770: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x778: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x780: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x788: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x790: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x798: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x7c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x7e0: */  0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
/* 0x7e8: */  0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
/* 0x7f0: */  0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
/* 0x7f8: */  0x3e, 0x3e, 0x3f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f,
/* 0x800: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x808: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x810: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x818: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x820: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x828: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x830: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x838: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x840: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x848: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x850: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x858: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x860: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x868: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x870: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x878: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x880: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x888: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x890: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x898: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x8c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x8f8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0x900: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x908: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x910: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x918: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x920: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x928: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x930: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x938: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x940: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x948: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x950: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x958: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x960: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x968: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x970: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x978: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0x980: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x988: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x990: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x998: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9a0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9a8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9b0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9b8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0x9c0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9c8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9d0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9d8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9e0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9e8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9f0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0x9f8: */  0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f,
/* 0xa00: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa08: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa10: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa18: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa20: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa28: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa30: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa38: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa40: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa48: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa50: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa58: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa60: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa68: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa70: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa78: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0xa80: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa88: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa90: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xa98: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xaa0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xaa8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xab0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xab8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xac0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xac8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xad0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xad8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xae0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xae8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xaf0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xaf8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0xb00: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb08: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb10: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb18: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb20: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb28: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb30: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb38: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xb40: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb48: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb50: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb58: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb60: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb68: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb70: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb78: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0xb80: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb88: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb90: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xb98: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xba0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xba8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbb0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbb8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xbc0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbc8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbd0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbd8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbe0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbe8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xbf0: */  0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
/* 0xbf8: */  0x1e, 0x1e, 0x1e, 0x1e, 0x1f, 0x1f, 0x3f, 0x3f,
/* 0xc00: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc08: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc10: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc18: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc20: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc28: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc30: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc38: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc40: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc48: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc50: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc58: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc60: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc68: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc70: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc78: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0xc80: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc88: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc90: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xc98: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xca0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xca8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcb0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcb8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xcc0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcc8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcd0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcd8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xce0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xce8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcf0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xcf8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0xd00: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd08: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd10: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd18: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd20: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd28: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd30: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd38: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xd40: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd48: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd50: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd58: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd60: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd68: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd70: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd78: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0xd80: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd88: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd90: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xd98: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xda0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xda8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdb0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdb8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xdc0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdc8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdd0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdd8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xde0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xde8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdf0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xdf8: */  0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x1f,
/* 0xe00: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe08: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe10: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe18: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe20: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe28: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe30: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe38: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe40: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe48: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe50: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe58: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe60: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe68: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe70: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe78: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0xe80: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe88: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe90: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xe98: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xea0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xea8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xeb0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xeb8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xec0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xec8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xed0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xed8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xee0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xee8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xef0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xef8: */  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07,
/* 0xf00: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf08: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf10: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf18: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf20: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf28: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf30: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf38: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xf40: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf48: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf50: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf58: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf60: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf68: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf70: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf78: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
/* 0xf80: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf88: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf90: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xf98: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfa0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfa8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfb0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfb8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/* 0xfc0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfc8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfd0: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfd8: */  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* 0xfe0: */  0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
/* 0xfe8: */  0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
/* 0xff0: */  0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c,
/* 0xff8: */  0x3e, 0x3e, 0x3f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f,
};
