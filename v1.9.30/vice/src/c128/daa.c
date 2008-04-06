/*
 * daa.c
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

#include "types.h"

BYTE daa_reg_a[2048] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
    0x28,0x29,0x30,0x31,0x32,0x33,0x34,0x35,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0x40,0x41,0x42,0x43,0x44,0x45,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x50,0x51,0x52,0x53,0x54,0x55,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
    0x58,0x59,0x60,0x61,0x62,0x63,0x64,0x65,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
    0x68,0x69,0x70,0x71,0x72,0x73,0x74,0x75,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
    0x78,0x79,0x80,0x81,0x82,0x83,0x84,0x85,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x90,0x91,0x92,0x93,0x94,0x95,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
    0x98,0x99,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
    0x28,0x29,0x30,0x31,0x32,0x33,0x34,0x35,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0x40,0x41,0x42,0x43,0x44,0x45,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x50,0x51,0x52,0x53,0x54,0x55,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
    0x58,0x59,0x60,0x61,0x62,0x63,0x64,0x65,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
    0x68,0x69,0x70,0x71,0x72,0x73,0x74,0x75,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
    0x78,0x79,0x80,0x81,0x82,0x83,0x84,0x85,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x90,0x91,0x92,0x93,0x94,0x95,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
    0x98,0x99,0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
    0xA8,0xA9,0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,
    0xB8,0xB9,0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
    0xC8,0xC9,0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,
    0xD8,0xD9,0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
    0xE8,0xE9,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
    0xF8,0xF9,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
    0x28,0x29,0x30,0x31,0x32,0x33,0x34,0x35,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0x40,0x41,0x42,0x43,0x44,0x45,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x50,0x51,0x52,0x53,0x54,0x55,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
    0x58,0x59,0x60,0x61,0x62,0x63,0x64,0x65,
    0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
    0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,
    0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,
    0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,
    0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,
    0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,
    0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,
    0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,
    0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,
    0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,
    0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,
    0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,
    0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,
    0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,
    0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,
    0x7E,0x7F,0x80,0x81,0x82,0x83,0x84,0x85,
    0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,
    0x8E,0x8F,0x90,0x91,0x92,0x93,0x94,0x95,
    0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,
    0x9E,0x9F,0x00,0x01,0x02,0x03,0x04,0x05,
    0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
    0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,
    0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,
    0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,
    0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,
    0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,
    0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,
    0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,
    0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,
    0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,
    0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,
    0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,
    0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,
    0x6E,0x6F,0x70,0x71,0x72,0x73,0x74,0x75,
    0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,
    0x7E,0x7F,0x80,0x81,0x82,0x83,0x84,0x85,
    0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,
    0x8E,0x8F,0x90,0x91,0x92,0x93,0x94,0x95,
    0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,
    0x9E,0x9F,0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,
    0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,
    0xAE,0xAF,0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,
    0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,
    0xBE,0xBF,0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,
    0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,
    0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,
    0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,
    0xDE,0xDF,0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,
    0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,
    0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,
    0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,
    0xFE,0xFF,0x00,0x01,0x02,0x03,0x04,0x05,
    0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,
    0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,0x15,
    0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,
    0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,
    0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,
    0x2E,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,
    0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,
    0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,
    0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,
    0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,
    0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,
    0x5E,0x5F,0x60,0x61,0x62,0x63,0x64,0x65,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x04,0x05,0x06,0x07,0x08,0x09,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x14,0x15,0x16,0x17,0x18,0x19,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
    0x28,0x29,0x24,0x25,0x26,0x27,0x28,0x29,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0x34,0x35,0x36,0x37,0x38,0x39,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x44,0x45,0x46,0x47,0x48,0x49,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
    0x58,0x59,0x54,0x55,0x56,0x57,0x58,0x59,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
    0x68,0x69,0x64,0x65,0x66,0x67,0x68,0x69,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
    0x78,0x79,0x74,0x75,0x76,0x77,0x78,0x79,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x84,0x85,0x86,0x87,0x88,0x89,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
    0x98,0x99,0x34,0x35,0x36,0x37,0x38,0x39,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x44,0x45,0x46,0x47,0x48,0x49,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
    0x58,0x59,0x54,0x55,0x56,0x57,0x58,0x59,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
    0x68,0x69,0x64,0x65,0x66,0x67,0x68,0x69,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
    0x78,0x79,0x74,0x75,0x76,0x77,0x78,0x79,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x84,0x85,0x86,0x87,0x88,0x89,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
    0x98,0x99,0x94,0x95,0x96,0x97,0x98,0x99,
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,
    0xA8,0xA9,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,
    0xB8,0xB9,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,
    0xC8,0xC9,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,
    0xD8,0xD9,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
    0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
    0xE8,0xE9,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,
    0xF8,0xF9,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x04,0x05,0x06,0x07,0x08,0x09,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x14,0x15,0x16,0x17,0x18,0x19,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
    0x28,0x29,0x24,0x25,0x26,0x27,0x28,0x29,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
    0x38,0x39,0x34,0x35,0x36,0x37,0x38,0x39,
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
    0x48,0x49,0x44,0x45,0x46,0x47,0x48,0x49,
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
    0x58,0x59,0x54,0x55,0x56,0x57,0x58,0x59,
    0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
    0x68,0x69,0x64,0x65,0x66,0x67,0x68,0x69,
    0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
    0x78,0x79,0x74,0x75,0x76,0x77,0x78,0x79,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
    0x88,0x89,0x84,0x85,0x86,0x87,0x88,0x89,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
    0x98,0x99,0x94,0x95,0x96,0x97,0x98,0x99,
    0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,0x00,0x01,
    0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,
    0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
    0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
    0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
    0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,
    0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
    0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,
    0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
    0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,
    0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
    0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,
    0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
    0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,
    0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
    0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x80,0x81,
    0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
    0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,
    0x92,0x93,0x34,0x35,0x36,0x37,0x38,0x39,
    0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,
    0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
    0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,
    0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
    0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,
    0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
    0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,
    0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
    0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x80,0x81,
    0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
    0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,
    0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,
    0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,0xA0,0xA1,
    0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,
    0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,
    0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,
    0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,
    0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,
    0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,0xD1,
    0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,
    0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE0,0xE1,
    0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,
    0xEA,0xEB,0xEC,0xED,0xEE,0xEF,0xF0,0xF1,
    0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,
    0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,0x00,0x01,
    0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
    0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,
    0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
    0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
    0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,
    0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,
    0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
    0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,
    0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
    0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,
    0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
    0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,0x60,0x61,
    0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
    0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70,0x71,
    0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
    0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,0x80,0x81,
    0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
    0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,0x90,0x91,
    0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99 };

BYTE daa_reg_f[2048] = {
    0x44,0x00,0x00,0x04,0x00,0x04,0x04,0x00,
    0x08,0x0C,0x10,0x14,0x14,0x10,0x14,0x10,
    0x00,0x04,0x04,0x00,0x04,0x00,0x00,0x04,
    0x0C,0x08,0x30,0x34,0x34,0x30,0x34,0x30,
    0x20,0x24,0x24,0x20,0x24,0x20,0x20,0x24,
    0x2C,0x28,0x34,0x30,0x30,0x34,0x30,0x34,
    0x24,0x20,0x20,0x24,0x20,0x24,0x24,0x20,
    0x28,0x2C,0x10,0x14,0x14,0x10,0x14,0x10,
    0x00,0x04,0x04,0x00,0x04,0x00,0x00,0x04,
    0x0C,0x08,0x14,0x10,0x10,0x14,0x10,0x14,
    0x04,0x00,0x00,0x04,0x00,0x04,0x04,0x00,
    0x08,0x0C,0x34,0x30,0x30,0x34,0x30,0x34,
    0x24,0x20,0x20,0x24,0x20,0x24,0x24,0x20,
    0x28,0x2C,0x30,0x34,0x34,0x30,0x34,0x30,
    0x20,0x24,0x24,0x20,0x24,0x20,0x20,0x24,
    0x2C,0x28,0x90,0x94,0x94,0x90,0x94,0x90,
    0x80,0x84,0x84,0x80,0x84,0x80,0x80,0x84,
    0x8C,0x88,0x94,0x90,0x90,0x94,0x90,0x94,
    0x84,0x80,0x80,0x84,0x80,0x84,0x84,0x80,
    0x88,0x8C,0x55,0x11,0x11,0x15,0x11,0x15,
    0x45,0x01,0x01,0x05,0x01,0x05,0x05,0x01,
    0x09,0x0D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x05,0x01,0x05,0x01,0x01,0x05,
    0x0D,0x09,0x31,0x35,0x35,0x31,0x35,0x31,
    0x21,0x25,0x25,0x21,0x25,0x21,0x21,0x25,
    0x2D,0x29,0x35,0x31,0x31,0x35,0x31,0x35,
    0x25,0x21,0x21,0x25,0x21,0x25,0x25,0x21,
    0x29,0x2D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x05,0x01,0x05,0x01,0x01,0x05,
    0x0D,0x09,0x15,0x11,0x11,0x15,0x11,0x15,
    0x05,0x01,0x01,0x05,0x01,0x05,0x05,0x01,
    0x09,0x0D,0x35,0x31,0x31,0x35,0x31,0x35,
    0x25,0x21,0x21,0x25,0x21,0x25,0x25,0x21,
    0x29,0x2D,0x31,0x35,0x35,0x31,0x35,0x31,
    0x21,0x25,0x25,0x21,0x25,0x21,0x21,0x25,
    0x2D,0x29,0x91,0x95,0x95,0x91,0x95,0x91,
    0x81,0x85,0x85,0x81,0x85,0x81,0x81,0x85,
    0x8D,0x89,0x95,0x91,0x91,0x95,0x91,0x95,
    0x85,0x81,0x81,0x85,0x81,0x85,0x85,0x81,
    0x89,0x8D,0xB5,0xB1,0xB1,0xB5,0xB1,0xB5,
    0xA5,0xA1,0xA1,0xA5,0xA1,0xA5,0xA5,0xA1,
    0xA9,0xAD,0xB1,0xB5,0xB5,0xB1,0xB5,0xB1,
    0xA1,0xA5,0xA5,0xA1,0xA5,0xA1,0xA1,0xA5,
    0xAD,0xA9,0x95,0x91,0x91,0x95,0x91,0x95,
    0x85,0x81,0x81,0x85,0x81,0x85,0x85,0x81,
    0x89,0x8D,0x91,0x95,0x95,0x91,0x95,0x91,
    0x81,0x85,0x85,0x81,0x85,0x81,0x81,0x85,
    0x8D,0x89,0xB1,0xB5,0xB5,0xB1,0xB5,0xB1,
    0xA1,0xA5,0xA5,0xA1,0xA5,0xA1,0xA1,0xA5,
    0xAD,0xA9,0xB5,0xB1,0xB1,0xB5,0xB1,0xB5,
    0xA5,0xA1,0xA1,0xA5,0xA1,0xA5,0xA5,0xA1,
    0xA9,0xAD,0x55,0x11,0x11,0x15,0x11,0x15,
    0x45,0x01,0x01,0x05,0x01,0x05,0x05,0x01,
    0x09,0x0D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x05,0x01,0x05,0x01,0x01,0x05,
    0x0D,0x09,0x31,0x35,0x35,0x31,0x35,0x31,
    0x21,0x25,0x25,0x21,0x25,0x21,0x21,0x25,
    0x2D,0x29,0x35,0x31,0x31,0x35,0x31,0x35,
    0x25,0x21,0x21,0x25,0x21,0x25,0x25,0x21,
    0x29,0x2D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x05,0x01,0x05,0x01,0x01,0x05,
    0x0D,0x09,0x15,0x11,0x11,0x15,0x11,0x15,
    0x05,0x01,0x01,0x05,0x01,0x05,0x05,0x01,
    0x09,0x0D,0x35,0x31,0x31,0x35,0x31,0x35,
    0x04,0x00,0x08,0x0C,0x0C,0x08,0x0C,0x08,
    0x08,0x0C,0x10,0x14,0x14,0x10,0x14,0x10,
    0x00,0x04,0x0C,0x08,0x08,0x0C,0x08,0x0C,
    0x0C,0x08,0x30,0x34,0x34,0x30,0x34,0x30,
    0x20,0x24,0x2C,0x28,0x28,0x2C,0x28,0x2C,
    0x2C,0x28,0x34,0x30,0x30,0x34,0x30,0x34,
    0x24,0x20,0x28,0x2C,0x2C,0x28,0x2C,0x28,
    0x28,0x2C,0x10,0x14,0x14,0x10,0x14,0x10,
    0x00,0x04,0x0C,0x08,0x08,0x0C,0x08,0x0C,
    0x0C,0x08,0x14,0x10,0x10,0x14,0x10,0x14,
    0x04,0x00,0x08,0x0C,0x0C,0x08,0x0C,0x08,
    0x08,0x0C,0x34,0x30,0x30,0x34,0x30,0x34,
    0x24,0x20,0x28,0x2C,0x2C,0x28,0x2C,0x28,
    0x28,0x2C,0x30,0x34,0x34,0x30,0x34,0x30,
    0x20,0x24,0x2C,0x28,0x28,0x2C,0x28,0x2C,
    0x2C,0x28,0x90,0x94,0x94,0x90,0x94,0x90,
    0x80,0x84,0x8C,0x88,0x88,0x8C,0x88,0x8C,
    0x8C,0x88,0x94,0x90,0x90,0x94,0x90,0x94,
    0x84,0x80,0x88,0x8C,0x8C,0x88,0x8C,0x88,
    0x88,0x8C,0x55,0x11,0x11,0x15,0x11,0x15,
    0x05,0x01,0x09,0x0D,0x0D,0x09,0x0D,0x09,
    0x09,0x0D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x0D,0x09,0x09,0x0D,0x09,0x0D,
    0x0D,0x09,0x31,0x35,0x35,0x31,0x35,0x31,
    0x21,0x25,0x2D,0x29,0x29,0x2D,0x29,0x2D,
    0x2D,0x29,0x35,0x31,0x31,0x35,0x31,0x35,
    0x25,0x21,0x29,0x2D,0x2D,0x29,0x2D,0x29,
    0x29,0x2D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x0D,0x09,0x09,0x0D,0x09,0x0D,
    0x0D,0x09,0x15,0x11,0x11,0x15,0x11,0x15,
    0x05,0x01,0x09,0x0D,0x0D,0x09,0x0D,0x09,
    0x09,0x0D,0x35,0x31,0x31,0x35,0x31,0x35,
    0x25,0x21,0x29,0x2D,0x2D,0x29,0x2D,0x29,
    0x29,0x2D,0x31,0x35,0x35,0x31,0x35,0x31,
    0x21,0x25,0x2D,0x29,0x29,0x2D,0x29,0x2D,
    0x2D,0x29,0x91,0x95,0x95,0x91,0x95,0x91,
    0x81,0x85,0x8D,0x89,0x89,0x8D,0x89,0x8D,
    0x8D,0x89,0x95,0x91,0x91,0x95,0x91,0x95,
    0x85,0x81,0x89,0x8D,0x8D,0x89,0x8D,0x89,
    0x89,0x8D,0xB5,0xB1,0xB1,0xB5,0xB1,0xB5,
    0xA5,0xA1,0xA9,0xAD,0xAD,0xA9,0xAD,0xA9,
    0xA9,0xAD,0xB1,0xB5,0xB5,0xB1,0xB5,0xB1,
    0xA1,0xA5,0xAD,0xA9,0xA9,0xAD,0xA9,0xAD,
    0xAD,0xA9,0x95,0x91,0x91,0x95,0x91,0x95,
    0x85,0x81,0x89,0x8D,0x8D,0x89,0x8D,0x89,
    0x89,0x8D,0x91,0x95,0x95,0x91,0x95,0x91,
    0x81,0x85,0x8D,0x89,0x89,0x8D,0x89,0x8D,
    0x8D,0x89,0xB1,0xB5,0xB5,0xB1,0xB5,0xB1,
    0xA1,0xA5,0xAD,0xA9,0xA9,0xAD,0xA9,0xAD,
    0xAD,0xA9,0xB5,0xB1,0xB1,0xB5,0xB1,0xB5,
    0xA5,0xA1,0xA9,0xAD,0xAD,0xA9,0xAD,0xA9,
    0xA9,0xAD,0x55,0x11,0x11,0x15,0x11,0x15,
    0x05,0x01,0x09,0x0D,0x0D,0x09,0x0D,0x09,
    0x09,0x0D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x0D,0x09,0x09,0x0D,0x09,0x0D,
    0x0D,0x09,0x31,0x35,0x35,0x31,0x35,0x31,
    0x21,0x25,0x2D,0x29,0x29,0x2D,0x29,0x2D,
    0x2D,0x29,0x35,0x31,0x31,0x35,0x31,0x35,
    0x25,0x21,0x29,0x2D,0x2D,0x29,0x2D,0x29,
    0x29,0x2D,0x11,0x15,0x15,0x11,0x15,0x11,
    0x01,0x05,0x0D,0x09,0x09,0x0D,0x09,0x0D,
    0x0D,0x09,0x15,0x11,0x11,0x15,0x11,0x15,
    0x05,0x01,0x09,0x0D,0x0D,0x09,0x0D,0x09,
    0x09,0x0D,0x35,0x31,0x31,0x35,0x31,0x35,
    0x46,0x02,0x02,0x06,0x02,0x06,0x06,0x02,
    0x0A,0x0E,0x02,0x06,0x06,0x02,0x0A,0x0E,
    0x02,0x06,0x06,0x02,0x06,0x02,0x02,0x06,
    0x0E,0x0A,0x06,0x02,0x02,0x06,0x0E,0x0A,
    0x22,0x26,0x26,0x22,0x26,0x22,0x22,0x26,
    0x2E,0x2A,0x26,0x22,0x22,0x26,0x2E,0x2A,
    0x26,0x22,0x22,0x26,0x22,0x26,0x26,0x22,
    0x2A,0x2E,0x22,0x26,0x26,0x22,0x2A,0x2E,
    0x02,0x06,0x06,0x02,0x06,0x02,0x02,0x06,
    0x0E,0x0A,0x06,0x02,0x02,0x06,0x0E,0x0A,
    0x06,0x02,0x02,0x06,0x02,0x06,0x06,0x02,
    0x0A,0x0E,0x02,0x06,0x06,0x02,0x0A,0x0E,
    0x26,0x22,0x22,0x26,0x22,0x26,0x26,0x22,
    0x2A,0x2E,0x22,0x26,0x26,0x22,0x2A,0x2E,
    0x22,0x26,0x26,0x22,0x26,0x22,0x22,0x26,
    0x2E,0x2A,0x26,0x22,0x22,0x26,0x2E,0x2A,
    0x82,0x86,0x86,0x82,0x86,0x82,0x82,0x86,
    0x8E,0x8A,0x86,0x82,0x82,0x86,0x8E,0x8A,
    0x86,0x82,0x82,0x86,0x82,0x86,0x86,0x82,
    0x8A,0x8E,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x03,0x07,0x07,0x03,0x07,0x03,0x03,0x07,
    0x0F,0x0B,0x07,0x03,0x03,0x07,0x0F,0x0B,
    0x07,0x03,0x03,0x07,0x03,0x07,0x07,0x03,
    0x0B,0x0F,0x03,0x07,0x07,0x03,0x0B,0x0F,
    0x27,0x23,0x23,0x27,0x23,0x27,0x27,0x23,
    0x2B,0x2F,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x23,0x27,0x27,0x23,0x27,0x23,0x23,0x27,
    0x2F,0x2B,0x27,0x23,0x23,0x27,0x2F,0x2B,
    0x83,0x87,0x87,0x83,0x87,0x83,0x83,0x87,
    0x8F,0x8B,0x87,0x83,0x83,0x87,0x8F,0x8B,
    0x87,0x83,0x83,0x87,0x83,0x87,0x87,0x83,
    0x8B,0x8F,0x83,0x87,0x87,0x83,0x8B,0x8F,
    0xA7,0xA3,0xA3,0xA7,0xA3,0xA7,0xA7,0xA3,
    0xAB,0xAF,0xA3,0xA7,0xA7,0xA3,0xAB,0xAF,
    0xA3,0xA7,0xA7,0xA3,0xA7,0xA3,0xA3,0xA7,
    0xAF,0xAB,0xA7,0xA3,0xA3,0xA7,0xAF,0xAB,
    0x87,0x83,0x83,0x87,0x83,0x87,0x87,0x83,
    0x8B,0x8F,0x83,0x87,0x87,0x83,0x8B,0x8F,
    0x83,0x87,0x87,0x83,0x87,0x83,0x83,0x87,
    0x8F,0x8B,0x87,0x83,0x83,0x87,0x8F,0x8B,
    0xA3,0xA7,0xA7,0xA3,0xA7,0xA3,0xA3,0xA7,
    0xAF,0xAB,0xA7,0xA3,0xA3,0xA7,0xAF,0xAB,
    0xA7,0xA3,0xA3,0xA7,0xA3,0xA7,0xA7,0xA3,
    0xAB,0xAF,0xA3,0xA7,0xA7,0xA3,0xAB,0xAF,
    0x47,0x03,0x03,0x07,0x03,0x07,0x07,0x03,
    0x0B,0x0F,0x03,0x07,0x07,0x03,0x0B,0x0F,
    0x03,0x07,0x07,0x03,0x07,0x03,0x03,0x07,
    0x0F,0x0B,0x07,0x03,0x03,0x07,0x0F,0x0B,
    0x23,0x27,0x27,0x23,0x27,0x23,0x23,0x27,
    0x2F,0x2B,0x27,0x23,0x23,0x27,0x2F,0x2B,
    0x27,0x23,0x23,0x27,0x23,0x27,0x27,0x23,
    0x2B,0x2F,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x03,0x07,0x07,0x03,0x07,0x03,0x03,0x07,
    0x0F,0x0B,0x07,0x03,0x03,0x07,0x0F,0x0B,
    0x07,0x03,0x03,0x07,0x03,0x07,0x07,0x03,
    0x0B,0x0F,0x03,0x07,0x07,0x03,0x0B,0x0F,
    0x27,0x23,0x23,0x27,0x23,0x27,0x27,0x23,
    0x2B,0x2F,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x23,0x27,0x27,0x23,0x27,0x23,0x23,0x27,
    0x2F,0x2B,0x27,0x23,0x23,0x27,0x2F,0x2B,
    0x83,0x87,0x87,0x83,0x87,0x83,0x83,0x87,
    0x8F,0x8B,0x87,0x83,0x83,0x87,0x8F,0x8B,
    0x87,0x83,0x83,0x87,0x83,0x87,0x87,0x83,
    0x8B,0x8F,0x83,0x87,0x87,0x83,0x8B,0x8F,
    0xBE,0xBA,0xBE,0xBA,0xBA,0xBE,0x46,0x02,
    0x02,0x06,0x02,0x06,0x06,0x02,0x0A,0x0E,
    0x1E,0x1A,0x1E,0x1A,0x1A,0x1E,0x02,0x06,
    0x06,0x02,0x06,0x02,0x02,0x06,0x0E,0x0A,
    0x1A,0x1E,0x1A,0x1E,0x1E,0x1A,0x22,0x26,
    0x26,0x22,0x26,0x22,0x22,0x26,0x2E,0x2A,
    0x3A,0x3E,0x3A,0x3E,0x3E,0x3A,0x26,0x22,
    0x22,0x26,0x22,0x26,0x26,0x22,0x2A,0x2E,
    0x3E,0x3A,0x3E,0x3A,0x3A,0x3E,0x02,0x06,
    0x06,0x02,0x06,0x02,0x02,0x06,0x0E,0x0A,
    0x1A,0x1E,0x1A,0x1E,0x1E,0x1A,0x06,0x02,
    0x02,0x06,0x02,0x06,0x06,0x02,0x0A,0x0E,
    0x1E,0x1A,0x1E,0x1A,0x1A,0x1E,0x26,0x22,
    0x22,0x26,0x22,0x26,0x26,0x22,0x2A,0x2E,
    0x3E,0x3A,0x3E,0x3A,0x3A,0x3E,0x22,0x26,
    0x26,0x22,0x26,0x22,0x22,0x26,0x2E,0x2A,
    0x3A,0x3E,0x3A,0x3E,0x3E,0x3A,0x82,0x86,
    0x86,0x82,0x86,0x82,0x82,0x86,0x8E,0x8A,
    0x9A,0x9E,0x9A,0x9E,0x9E,0x9A,0x86,0x82,
    0x82,0x86,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x3F,0x3B,0x3F,0x3B,0x3B,0x3F,0x03,0x07,
    0x07,0x03,0x07,0x03,0x03,0x07,0x0F,0x0B,
    0x1B,0x1F,0x1B,0x1F,0x1F,0x1B,0x07,0x03,
    0x03,0x07,0x03,0x07,0x07,0x03,0x0B,0x0F,
    0x1F,0x1B,0x1F,0x1B,0x1B,0x1F,0x27,0x23,
    0x23,0x27,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x3F,0x3B,0x3F,0x3B,0x3B,0x3F,0x23,0x27,
    0x27,0x23,0x27,0x23,0x23,0x27,0x2F,0x2B,
    0x3B,0x3F,0x3B,0x3F,0x3F,0x3B,0x83,0x87,
    0x87,0x83,0x87,0x83,0x83,0x87,0x8F,0x8B,
    0x9B,0x9F,0x9B,0x9F,0x9F,0x9B,0x87,0x83,
    0x83,0x87,0x83,0x87,0x87,0x83,0x8B,0x8F,
    0x9F,0x9B,0x9F,0x9B,0x9B,0x9F,0xA7,0xA3,
    0xA3,0xA7,0xA3,0xA7,0xA7,0xA3,0xAB,0xAF,
    0xBF,0xBB,0xBF,0xBB,0xBB,0xBF,0xA3,0xA7,
    0xA7,0xA3,0xA7,0xA3,0xA3,0xA7,0xAF,0xAB,
    0xBB,0xBF,0xBB,0xBF,0xBF,0xBB,0x87,0x83,
    0x83,0x87,0x83,0x87,0x87,0x83,0x8B,0x8F,
    0x9F,0x9B,0x9F,0x9B,0x9B,0x9F,0x83,0x87,
    0x87,0x83,0x87,0x83,0x83,0x87,0x8F,0x8B,
    0x9B,0x9F,0x9B,0x9F,0x9F,0x9B,0xA3,0xA7,
    0xA7,0xA3,0xA7,0xA3,0xA3,0xA7,0xAF,0xAB,
    0xBB,0xBF,0xBB,0xBF,0xBF,0xBB,0xA7,0xA3,
    0xA3,0xA7,0xA3,0xA7,0xA7,0xA3,0xAB,0xAF,
    0xBF,0xBB,0xBF,0xBB,0xBB,0xBF,0x47,0x03,
    0x03,0x07,0x03,0x07,0x07,0x03,0x0B,0x0F,
    0x1F,0x1B,0x1F,0x1B,0x1B,0x1F,0x03,0x07,
    0x07,0x03,0x07,0x03,0x03,0x07,0x0F,0x0B,
    0x1B,0x1F,0x1B,0x1F,0x1F,0x1B,0x23,0x27,
    0x27,0x23,0x27,0x23,0x23,0x27,0x2F,0x2B,
    0x3B,0x3F,0x3B,0x3F,0x3F,0x3B,0x27,0x23,
    0x23,0x27,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x3F,0x3B,0x3F,0x3B,0x3B,0x3F,0x03,0x07,
    0x07,0x03,0x07,0x03,0x03,0x07,0x0F,0x0B,
    0x1B,0x1F,0x1B,0x1F,0x1F,0x1B,0x07,0x03,
    0x03,0x07,0x03,0x07,0x07,0x03,0x0B,0x0F,
    0x1F,0x1B,0x1F,0x1B,0x1B,0x1F,0x27,0x23,
    0x23,0x27,0x23,0x27,0x27,0x23,0x2B,0x2F,
    0x3F,0x3B,0x3F,0x3B,0x3B,0x3F,0x23,0x27,
    0x27,0x23,0x27,0x23,0x23,0x27,0x2F,0x2B,
    0x3B,0x3F,0x3B,0x3F,0x3F,0x3B,0x83,0x87,
    0x87,0x83,0x87,0x83,0x83,0x87,0x8F,0x8B,
    0x9B,0x9F,0x9B,0x9F,0x9F,0x9B,0x87,0x83,
    0x83,0x87,0x83,0x87,0x87,0x83,0x8B,0x8F
};

