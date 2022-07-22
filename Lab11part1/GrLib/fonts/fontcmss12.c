/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//*****************************************************************************
//
// This file is generated by ftrasterize; DO NOT EDIT BY HAND!
//
//*****************************************************************************

#include "Grlib/grlib/grlib.h"

//*****************************************************************************
//
// Details of this font:
//     Style: cmss
//     Size: 12 point
//     Bold: no
//     Italic: no
//     Memory usage: 1500 bytes
//
//*****************************************************************************

//*****************************************************************************
//
// The compressed data for the 12 point Cmss font.
//
//*****************************************************************************
static const uint8_t g_pucCmss12Data[1300] =
{
      5,   5,   0,   8,  16,  10,   2,  65,  17,  17,  17,  17,
     17,  49, 112,   9,   4, 129,  17,  17,  17, 240, 240, 112,
     21,   9, 240, 145,  81,  17,  81,  33,  56,  49,  33,  56,
     33,  33,  81,  33,  81,  33,  81, 240,  16,  16,   5,  17,
     66,  36,  18,  50,  50,  67,  33,  17,  33,  17,  19,  49,
    208,  28,   9, 161, 113,  17,  33,  49,  17,  17,  65,  17,
     17,  65,  18,  97,  17, 113,  20,  49,  17,  33,  33,  33,
     33,  33,  50, 240,  80,  20,   9, 240,  98,  97,  17,  97,
     17,  98, 114,  33,  34,  17,  33,  33,  50,  67,  19, 240,
    208,   6,   2,  65,  17, 240,  64,  15,   4,  33,  33,  49,
     33,  49,  49,  49,  49,  49,  49,  65,  49,  65,  16,   4,
      1,  65,  49,  65,  49,  49,  49,  49,  49,  49,  33,  49,
     33,  48,  10,   4,  17,  49,  49,  35,  19,  33, 240, 240,
     13,   8, 240, 193, 113, 113,  71,  65, 113, 113, 113, 240,
     80,   6,   2, 240,  49,  17,  80,   6,   4, 240, 211, 240,
     96,   5,   2, 240,  49, 112,  14,   5, 129,  49,  65,  65,
     49,  65,  65,  65,  49,  65,  65, 144,  18,   6, 211,  33,
     49,  17,  49,  17,  49,  17,  49,  17,  49,  17,  34,  35,
    240,  80,  11,   4, 145,  34,  49,  49,  49,  49,  49,  35,
    208,  13,   6, 211,  33,  49,  81,  81,  65,  65,  65,  84,
    240,  64,  13,   5, 178,  33,  33,  65,  34,  81,  65,  65,
     19, 240,  32,  15,   6, 241,  66,  49,  17,  49,  17,  33,
     33,  37,  65,  81, 240,  80,  13,   5, 164,  17,  65,  67,
     33,  33,  65,  65,  19, 240,  32,  16,   6, 227,  33,  65,
     81,  18,  34,  33,  17,  49,  17,  49,  35, 240,  80,  12,
      5, 164,  65,  49,  65,  65,  49,  65,  65, 240,  48,  17,
      6, 211,  33,  49,  17,  49,  35,  34,  18,  17,  49,  17,
     49,  35, 240,  80,  16,   6, 211,  33,  49,  17,  49,  17,
     49,  17,  49,  36,  65,  36, 240,  80,   5,   2, 161, 113,
    112,   6,   2, 161, 113,  17,  80,  11,   2, 129,  17,  17,
     17,  17,  17,  17,  17,  48,   9,   8, 240, 240, 167, 151,
    240, 240, 176,  12,   4, 240,  33,  49,  49,  49,  33,  49,
     49,  51,  80,  11,   5, 163,  81,  65,  49,  49,  65, 145,
    240,  48,  23,   7, 240,  19,  49,  19,  17,  17,  33,  17,
     17,  33,  17,  17,  33,  17,  17,  33,  33,  18,  67, 240,
    128,  19,   8, 240,  65,  97,  17,  81,  17,  81,  17,  65,
     49,  53,  49,  49,  33,  81, 240, 160,  17,   7, 229,  33,
     65,  17,  65,  21,  33,  50,  17,  65,  17,  50,  21, 240,
    128,  14,   7, 240,  20,  33,  49,  17,  97,  97,  97, 113,
    116, 240, 112,  18,   7, 228,  49,  49,  33,  65,  17,  65,
     17,  65,  17,  65,  17,  49,  36, 240, 144,  12,   6, 197,
     17,  81,  85,  17,  81,  81,  85, 240,  64,  12,   6, 197,
     17,  81,  84,  33,  81,  81,  81, 240, 128,  16,   7, 240,
     20,  33,  81,  97,  97,  50,  17,  65,  33,  49,  52, 240,
    112,  19,   7, 225,  65,  17,  65,  17,  65,  22,  17,  65,
     17,  65,  17,  65,  17,  65, 240, 112,  11,   2,  65,  17,
     17,  17,  17,  17,  17,  17, 112,  12,   5, 209,  65,  65,
     65,  65,  65,  65,  19, 240,  32,  20,   7, 225,  50,  17,
     34,  33,  33,  49,  17,  66,  17,  49,  34,  33,  49,  33,
     65, 240, 112,  12,   6, 193,  81,  81,  81,  81,  81,  81,
     85, 240,  64,  29,   9, 240,  50,  66,  18,  66,  18,  49,
     17,  17,  17,  33,  17,  17,  17,  33,  17,  17,  34,  33,
     17,  34,  33,  17,  34,  33, 240, 208,  24,   7, 226,  49,
     18,  49,  17,  17,  33,  17,  17,  33,  17,  33,  17,  17,
     33,  17,  17,  50,  17,  50, 240, 112,  19,   8, 240,  51,
     65,  49,  33,  81,  17,  81,  17,  81,  17,  81,  33,  49,
     67, 240, 192,  15,   6, 196,  33,  49,  17,  49,  17,  49,
     20,  33,  81,  81, 240, 128,  20,   8, 240,  51,  65,  49,
     33,  81,  17,  81,  17,  81,  17,  81,  33,  34,  68, 113,
    240,  48,  18,   7, 229,  33,  65,  17,  65,  21,  33,  33,
     49,  49,  33,  49,  33,  65, 240, 112,  12,   6, 211,  33,
     81,  98,  98,  81,  81,  35, 240,  80,  13,   8, 240,  23,
     65, 113, 113, 113, 113, 113, 113, 240, 208,  19,   7, 225,
     65,  17,  65,  17,  65,  17,  65,  17,  65,  17,  65,  18,
     34,  36, 240, 128,  19,   8, 240,  17,  81,  33,  65,  33,
     49,  49,  49,  65,  33,  65,  17,  98,  97, 240, 208,  30,
     11, 240, 113,  65,  49,  33,  34,  49,  33,  34,  49,  33,
     33,  17,  17,  49,  17,  33,  17,  66,  33,  17,  66,  34,
     82,  49, 240, 240,  96,  18,   7, 225,  49,  34,  33,  49,
     17,  81,  98,  65,  17,  49,  49,  33,  65, 240, 112,  17,
      8, 240,  17,  81,  33,  49,  50,  17,  81,  17,  97, 113,
    113, 113, 240, 208,  12,   6, 197,  81,  65,  65,  81,  65,
     65,  85, 240,  64,  14,   3,  50,  17,  33,  33,  33,  33,
     33,  33,  33,  33,  33,  34,   9,   4, 129,  17,  17,  17,
    240, 240, 112,  14,   3,  50,  33,  33,  33,  33,  33,  33,
     33,  33,  33,  33,  18,   7,   4, 146,  17, 240, 240, 144,
      5,   2,  65, 240,  96,   6,   2,  65,  17, 240,  64,  11,
      5, 240, 178,  81,  35,  17,  33,  20, 240,  16,  15,   5,
    161,  65,  65,  67,  33,  33,  17,  33,  17,  33,  19, 240,
     32,  10,   5, 240, 179,  17,  65,  65,  83, 240,  16,  16,
      6, 240,  17,  81,  81,  36,  17,  49,  17,  49,  17,  49,
     36, 240,  64,  12,   6, 240, 240,  19,  33,  49,  21,  17,
    100, 240,  64,  12,   5, 194,  33,  65,  51,  49,  65,  65,
     65, 240,  48,  15,   6, 240, 240,  20,  33,  17,  49,  17,
     50,  68,  17,  49,  35, 128,  16,   5, 161,  65,  65,  68,
     17,  33,  17,  33,  17,  33,  17,  33, 240,  16,   9,   2,
     65,  81,  17,  17,  17,  17, 112,  11,   4, 161, 177,  49,
     49,  49,  49,  49,  19,  80,  16,   5, 161,  65,  65,  65,
     33,  17,  17,  35,  33,  17,  33,  33, 240,  16,  11,   2,
     65,  17,  17,  17,  17,  17,  17,  17, 112,  19,   8, 240,
    240, 167,  17,  33,  33,  17,  33,  33,  17,  33,  33,  17,
     33,  33, 240, 160,  14,   5, 240, 164,  17,  33,  17,  33,
     17,  33,  17,  33, 240,  16,  14,   6, 240, 240,  19,  33,
     49,  17,  49,  17,  49,  35, 240,  80,  14,   5, 240, 163,
     33,  33,  17,  33,  17,  33,  19,  33,  65, 144,  15,   6,
    240, 240,  20,  17,  49,  17,  49,  17,  49,  36,  81,  81,
    112,   9,   4, 240,  83,  17,  49,  49,  49, 240,  10,   5,
    240, 164,  17,  82,  81,  20, 240,  16,  12,   5, 240,  17,
     65,  52,  33,  65,  65,  67, 240,  16,  14,   5, 240, 161,
     33,  17,  33,  17,  33,  17,  33,  20, 240,  16,  14,   6,
    240, 241,  49,  33,  17,  49,  17,  49,  17,  65, 240,  96,
     19,   8, 240, 240, 161,  33,  33,  33,  18,  17,  34,  17,
     17,  34,  18,  65,  33, 240, 176,  14,   6, 240, 242,  17,
     49,  17,  65,  65,  17,  33,  49, 240,  64,  14,   6, 240,
    241,  49,  33,  17,  49,  17,  66,  65,  81,  50, 160,  10,
      5, 240, 164,  49,  49,  65,  52, 240,  16,   8,   7, 240,
    240, 198, 240, 240, 208,   8,  12,   0,   9,  11,   0,   9,
     16,   9,   5, 177,  17,  17,  17,   0,   5, 112,   6,   4,
    131, 240, 240, 176,
};

//*****************************************************************************
//
// The font definition for the 12 point Cmss font.
//
//*****************************************************************************
const Graphics_Font g_sFontCmss12 =
{
    //
    // The format of the font.
    //
    FONT_FMT_PIXEL_RLE,

    //
    // The maximum width of the font.
    //
    10,

    //
    // The height of the font.
    //
    13,

    //
    // The baseline of the font.
    //
    10,

    //
    // The offset to each character in the font.
    //
    {
           0,    5,   15,   24,   45,   61,   89,  109,
         115,  130,  146,  156,  169,  175,  181,  186,
         200,  218,  229,  242,  255,  270,  283,  299,
         311,  328,  344,  349,  355,  366,  375,  387,
         398,  421,  440,  457,  471,  489,  501,  513,
         529,  548,  559,  571,  591,  603,  632,  656,
         675,  690,  710,  728,  740,  753,  772,  791,
         821,  839,  856,  868,  882,  891,  905,  912,
         917,  923,  934,  949,  959,  975,  987,  999,
        1014, 1030, 1039, 1050, 1066, 1077, 1096, 1110,
        1124, 1138, 1153, 1162, 1172, 1184, 1198, 1212,
        1231, 1245, 1259, 1269, 1277, 1285, 1294,
    },

    //
    // A pointer to the actual font data
    //
    g_pucCmss12Data
};
