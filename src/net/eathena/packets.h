/*
 *  The ManaPlus Client
 *  Copyright (C) 2004-2009  The Mana World Development Team
 *  Copyright (C) 2009-2010  The Mana Developers
 *  Copyright (C) 2011-2015  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NET_EATHENA_PACKETS_H
#define NET_EATHENA_PACKETS_H

#if defined(__GXX_EXPERIMENTAL_CXX0X__)
#include <cstdint>
#else
#include <stdint.h>
#endif

namespace EAthena
{

/** Warning: buffers and other variables are shared,
    so there can be only one connection active at a time */

int16_t packet_lengths[] =
{
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
 10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0040
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,  50,   3,  -1,  55,  17,   3,  37,  46,  -1,  23,  -1,   3, 149,   3,   2,
  3,  28,  19,  11,   3,  -1,   9,   5,  55,  53,  58,  60,  44,   2,   6,   6,
// #0x0080
  7,   3,   2,   2,   2,   5,  16,  12,  10,   7,  29,  23,  -1,  -1,  -1,   0,
  7,  22,  28,   2,   6,  30,  -1,  -1,   3,  -1,  -1,   5,   9,  17,  17,   6,
 23,   6,   6,  -1,  -1,  -1,  -1,   8,   7,   6,   7,   4,   7,   0,  -1,   6,
  8,   8,   3,   3,  -1,   6,   6,  -1,   7,   6,   2,   5,   6,  44,   5,   3,
// #0x00C0
  7,   2,   6,   8,   6,   7,  -1,  -1,  -1,  -1,   3,   3,   6,   6,   2,  27,
  3,   4,   4,   2,  -1,  -1,   3,  -1,   6,  14,   3,  -1,  28,  29,  -1,  -1,
 30,  30,  26,   2,   6,  26,   3,   3,   8,  19,   5,   2,   3,   2,   2,   2,
  3,   2,   6,   8,  21,   8,   8,   2,   2,  26,   3,  -1,   6,  27,  30,  10,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0100
  2,   6,   6,  30,  79,  31,  10,  10,  -1,  -1,   4,   6,   6,   2,  11,  -1,
 10,  39,   4,  10,  31,  35,  10,  18,   2,  13,  15,  20,  68,   2,   3,  16,
  6,  14,  -1,  -1,  21,   8,   8,   8,   8,   8,   2,   2,   3,   4,   2,  -1,
  6,  86,   6,  -1,  -1,   7,  -1,   6,   3,  16,   4,   4,   4,   6,  24,  26,
// #0x0140
 22,  14,   6,  10,  23,  19,   6,  39,   8,   9,   6,  27,  -1,   2,   6,   6,
110,   6,  -1,  -1,  -1,  -1,  -1,   6,  -1,  54,  66,  54,  90,  42,   6,  42,
 -1,  -1,  -1,  -1,  -1,  30,  -1,   3,  14,   3,  30,  10,  43,  14, 186, 182,
 14,  30,  10,   3,  -1,   6, 106,  -1,   4,   5,   4,  -1,   6,   7,  -1,  -1,
// #0x0180
  6,   3,  106,  10,  10, 34,   0,   6,   8,   4,   4,   4,  29,  -1,  10,   6,
 90,  86,  24,   6,  30, 102,   9,   4,   8,   4,  14,  10,   4,   6,   2,   6,
  3,   3,  37,   5,  11,  26,  -1,   4,   4,   6,  10,  12,   6,  -1,   4,   4,
 11,   7,  -1,  67,  12,  18, 114,   6,   3,   6,  26,  26,  26,  26,   2,   3,
// #0x01C0
  2,  14,  10,  -1,  22,  22,   4,   2,  13,  97,   0,   9,   9,  29,   6,  28,
  8,  14,  10,  35,   6,   8,   4,  11,  54,  53,  60,   2,  -1,  47,  33,   6,
 30,   8,  34,  14,   2,   6,  26,   2,  28,  81,   6,  10,  26,   2,  -1,  -1,
 -1,  -1,  20,  10,  32,   9,  34,  14,   2,   6,  48,  56,  -1,   4,   5,  10,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0200
 26,  -1,   0,   0,  18,  26,  11,  34,   0,  36,  10,  19,  10,   0,  32,   0,
 22,  -1,  16,   0,  42,   6,   6,   0,   0, 282, 282,  10,  10,   0,   0,   0,
 -1,  -1,   0,   8,  10,   0, 282,   0,   0,  15,   0,   0,   0,  19,  71,   5,
 12,   0,   0,   0,   0,  -1,   0,   0, 282,   0,   4,   0,   6,   0,   0,   0,
// #0x0240
 -1,   0,  -1,   0,   0,   3,   0,   0,   0,   3,  70,   0,   0,   0,   0,   0,
  3,   0,  -1,   3,   0,   5,   5,   8,   0,   0,  -1,   0,   0,   4,   0,   6,
  6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0280
  0,   0,   0,   6,  14,   0,   0,  -1,   0,  12,  18,   0,   0,   0,   4,   0,
  4,   4,   0,  70,  10,   0,   0,   0,   8,   6,   0,  80,   0,  -1,   0,   0,
  0,   0,   8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,  -1,  -1, 107,   6,  -1,   0,   7,  22, 191,   0,   8,   0,   0,   0,   0,
// #0x02C0
  0,  -1,   0,   0,   0,  30,  30,   0,   0,   3,   0,  65,   4,  71,  10,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,   0,   0,
  0,  33,   0,   0,   0,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,
 10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0300
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0340
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  6,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0380
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x03C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0400
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   0,  25,
// #0x0440
  0,   4,   0,   0,   0,   0,  14,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0480
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x04C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0500
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0540
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0580
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x05C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0600
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0640
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0680
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x06C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0700
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0740
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0780
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x07C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   8, 268,   0,   0,   0,   0,   0,   0,
  0,  15,   8,   0,   6,   0,   8,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,  14,   0,   0,   0,   8,  25,  10,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0800
 -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  14,  20,
  3,   0,   8,  -1,  86,  -1,   6,   0,  -1,   0,   4,  10,  10,  22,   8,   0,
  0,   0,   0,   0,   6,   0,  -1,   3,   0,   0,   0,   0,   0,  -1,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,  66,   5,   0,   0,   6,  26,   0,
// #0x0840
 -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  19,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,  -1,  -1,  -1,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0880
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,  12,  10,   0,   0,   0,   0,   0,
// #0x08C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,  10,   0,   0,  -1,   6,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0900
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,
  0,   0,   0,   0,  -1,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0940
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 31,   0,   0,   0,   0,   0,   0,  14,   0,   0,  -1,   0,   0, 288,   0,   0,
// #0x0980
  0,   0,   0,  29,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 31,  -1,  -1,  -1,  -1,  -1,  -1,   0,   0,  11,   9,   8,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,  12,   0,  16,   0,  16,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x09C0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,  -1,   0,  -1,   0,   0,   0,   0,   0,   0,   0,   7,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0A00
269,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,  14,   0,   0,   0,   0,   0,   0,   7,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0A40
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0A80
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0AC0
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
//0    1    2    3    4    5    6    7    8    9    a    b    c    d    e    f
//0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15
// #0x0B00
 16,  -1,  10,  -1,  -1,  -1,  -1,   0,  27,   0,  -1,  -1,   0,   0,   0,   0,
 -1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
// #0x0B40
};

}  // namespace EAthena

#endif  // NET_EATHENA_PACKETS_H
