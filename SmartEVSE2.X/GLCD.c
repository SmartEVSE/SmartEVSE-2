/*
;	 Project:       Smart EVSE
;    Date:          21 February 2019
;
;
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
 */
#include <xc.h>
#include <stdlib.h>
#include "EVSE.h"
#include "GLCD.h"

// LCD font 8x5
const far unsigned char font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
    0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
    0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
    0x18, 0x3C, 0x7E, 0x3C, 0x18,
    0x1C, 0x57, 0x7D, 0x57, 0x1C,
    0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
    0x00, 0x18, 0x3C, 0x18, 0x00,
    0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
    0x00, 0x18, 0x24, 0x18, 0x00,
    0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
    0x30, 0x48, 0x3A, 0x06, 0x0E,
    0x26, 0x29, 0x79, 0x29, 0x26,
    0x40, 0x7F, 0x05, 0x05, 0x07,
    0x40, 0x7F, 0x05, 0x25, 0x3F,
    0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
    0x7F, 0x3E, 0x1C, 0x1C, 0x08,
    0x08, 0x1C, 0x1C, 0x3E, 0x7F,
    0x14, 0x22, 0x7F, 0x22, 0x14,
    0x5F, 0x5F, 0x00, 0x5F, 0x5F,
    0x06, 0x09, 0x7F, 0x01, 0x7F,
    0x00, 0x66, 0x89, 0x95, 0x6A,
    0x60, 0x60, 0x60, 0x60, 0x60,
    0x94, 0xA2, 0xFF, 0xA2, 0x94,
    0x08, 0x04, 0x7E, 0x04, 0x08,
    0x10, 0x20, 0x7E, 0x20, 0x10,
    0x08, 0x08, 0x2A, 0x1C, 0x08,
    0x08, 0x1C, 0x2A, 0x08, 0x08,
    0x1E, 0x10, 0x10, 0x10, 0x10,
    0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
    0x30, 0x38, 0x3E, 0x38, 0x30,
    0x06, 0x0E, 0x3E, 0x0E, 0x06,
    0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x5F, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x07, 0x00,
    0x14, 0x7F, 0x14, 0x7F, 0x14,
    0x24, 0x2A, 0x7F, 0x2A, 0x12,
    0x23, 0x13, 0x08, 0x64, 0x62,
    0x36, 0x49, 0x56, 0x20, 0x50,
    0x00, 0x08, 0x07, 0x03, 0x00,
    0x00, 0x1C, 0x22, 0x41, 0x00,
    0x00, 0x41, 0x22, 0x1C, 0x00,
    0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
    0x08, 0x08, 0x3E, 0x08, 0x08,
    0x00, 0x80, 0x70, 0x30, 0x00,
    0x08, 0x08, 0x08, 0x08, 0x08,
    0x00, 0x00, 0x60, 0x60, 0x00,
    0x20, 0x10, 0x08, 0x04, 0x02,
    0x3E, 0x51, 0x49, 0x45, 0x3E,
    0x00, 0x42, 0x7F, 0x40, 0x00,
    0x72, 0x49, 0x49, 0x49, 0x46,
    0x21, 0x41, 0x49, 0x4D, 0x33,
    0x18, 0x14, 0x12, 0x7F, 0x10,
    0x27, 0x45, 0x45, 0x45, 0x39,
    0x3C, 0x4A, 0x49, 0x49, 0x31,
    0x41, 0x21, 0x11, 0x09, 0x07,
    0x36, 0x49, 0x49, 0x49, 0x36,
    0x46, 0x49, 0x49, 0x29, 0x1E,
    0x00, 0x00, 0x14, 0x00, 0x00,
    0x00, 0x40, 0x34, 0x00, 0x00,
    0x00, 0x08, 0x14, 0x22, 0x41,
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x00, 0x41, 0x22, 0x14, 0x08,
    0x02, 0x01, 0x59, 0x09, 0x06,
    0x3E, 0x41, 0x5D, 0x59, 0x4E,
    0x7C, 0x12, 0x11, 0x12, 0x7C,
    0x7F, 0x49, 0x49, 0x49, 0x36,
    0x3E, 0x41, 0x41, 0x41, 0x22,
    0x7F, 0x41, 0x41, 0x41, 0x3E,
    0x7F, 0x49, 0x49, 0x49, 0x41,
    0x7F, 0x09, 0x09, 0x09, 0x01,
    0x3E, 0x41, 0x41, 0x51, 0x73,
    0x7F, 0x08, 0x08, 0x08, 0x7F,
    0x00, 0x41, 0x7F, 0x41, 0x00,
    0x20, 0x40, 0x41, 0x3F, 0x01,
    0x7F, 0x08, 0x14, 0x22, 0x41,
    0x7F, 0x40, 0x40, 0x40, 0x40,
    0x7F, 0x02, 0x1C, 0x02, 0x7F,
    0x7F, 0x04, 0x08, 0x10, 0x7F,
    0x3E, 0x41, 0x41, 0x41, 0x3E,
    0x7F, 0x09, 0x09, 0x09, 0x06,
    0x3E, 0x41, 0x51, 0x21, 0x5E,
    0x7F, 0x09, 0x19, 0x29, 0x46,
    0x26, 0x49, 0x49, 0x49, 0x32,
    0x03, 0x01, 0x7F, 0x01, 0x03,
    0x3F, 0x40, 0x40, 0x40, 0x3F,
    0x1F, 0x20, 0x40, 0x20, 0x1F,
    0x3F, 0x40, 0x38, 0x40, 0x3F,
    0x63, 0x14, 0x08, 0x14, 0x63,
    0x03, 0x04, 0x78, 0x04, 0x03,
    0x61, 0x59, 0x49, 0x4D, 0x43,
    0x00, 0x7F, 0x41, 0x41, 0x41,
    0x02, 0x04, 0x08, 0x10, 0x20,
    0x00, 0x41, 0x41, 0x41, 0x7F,
    0x04, 0x02, 0x01, 0x02, 0x04,
    0x40, 0x40, 0x40, 0x40, 0x40,
    0x00, 0x03, 0x07, 0x08, 0x00,
    0x20, 0x54, 0x54, 0x78, 0x40,
    0x7F, 0x28, 0x44, 0x44, 0x38,
    0x38, 0x44, 0x44, 0x44, 0x28,
    0x38, 0x44, 0x44, 0x28, 0x7F,
    0x38, 0x54, 0x54, 0x54, 0x18,
    0x00, 0x08, 0x7E, 0x09, 0x02,
    0x18, 0xA4, 0xA4, 0x9C, 0x78,
    0x7F, 0x08, 0x04, 0x04, 0x78,
    0x00, 0x44, 0x7D, 0x40, 0x00,
    0x20, 0x40, 0x40, 0x3D, 0x00,
    0x7F, 0x10, 0x28, 0x44, 0x00,
    0x00, 0x41, 0x7F, 0x40, 0x00,
    0x7C, 0x04, 0x78, 0x04, 0x78,
    0x7C, 0x08, 0x04, 0x04, 0x78,
    0x38, 0x44, 0x44, 0x44, 0x38,
    0xFC, 0x18, 0x24, 0x24, 0x18,
    0x18, 0x24, 0x24, 0x18, 0xFC,
    0x7C, 0x08, 0x04, 0x04, 0x08,
    0x48, 0x54, 0x54, 0x54, 0x24,
    0x04, 0x04, 0x3F, 0x44, 0x24,
    0x3C, 0x40, 0x40, 0x20, 0x7C,
    0x1C, 0x20, 0x40, 0x20, 0x1C,
    0x3C, 0x40, 0x30, 0x40, 0x3C,
    0x44, 0x28, 0x10, 0x28, 0x44,
    0x4C, 0x90, 0x90, 0x90, 0x7C,
    0x44, 0x64, 0x54, 0x4C, 0x44,
    0x00, 0x08, 0x36, 0x41, 0x00,
    0x00, 0x00, 0x77, 0x00, 0x00,
    0x00, 0x41, 0x36, 0x08, 0x00,
    0x02, 0x01, 0x02, 0x04, 0x02,
    0x3C, 0x26, 0x23, 0x26, 0x3C,
    0x1E, 0xA1, 0xA1, 0x61, 0x12,
    0x3A, 0x40, 0x40, 0x20, 0x7A,
    0x38, 0x54, 0x54, 0x55, 0x59,
    0x21, 0x55, 0x55, 0x79, 0x41,
    0x21, 0x54, 0x54, 0x78, 0x41,
    0x21, 0x55, 0x54, 0x78, 0x40,
    0x20, 0x54, 0x55, 0x79, 0x40,
    0x0C, 0x1E, 0x52, 0x72, 0x12,
    0x39, 0x55, 0x55, 0x55, 0x59,
    0x39, 0x54, 0x54, 0x54, 0x59,
    0x39, 0x55, 0x54, 0x54, 0x58,
    0x00, 0x00, 0x45, 0x7C, 0x41,
    0x00, 0x02, 0x45, 0x7D, 0x42,
    0x00, 0x01, 0x45, 0x7C, 0x40,
    0xF0, 0x29, 0x24, 0x29, 0xF0,
    0xF0, 0x28, 0x25, 0x28, 0xF0,
    0x7C, 0x54, 0x55, 0x45, 0x00,
    0x20, 0x54, 0x54, 0x7C, 0x54,
    0x7C, 0x0A, 0x09, 0x7F, 0x49,
    0x32, 0x49, 0x49, 0x49, 0x32,
    0x32, 0x48, 0x48, 0x48, 0x32,
    0x32, 0x4A, 0x48, 0x48, 0x30,
    0x3A, 0x41, 0x41, 0x21, 0x7A,
    0x3A, 0x42, 0x40, 0x20, 0x78,
    0x00, 0x9D, 0xA0, 0xA0, 0x7D,
    0x39, 0x44, 0x44, 0x44, 0x39,
    0x3D, 0x40, 0x40, 0x40, 0x3D,
    0x3C, 0x24, 0xFF, 0x24, 0x24,
    0x48, 0x7E, 0x49, 0x43, 0x66,
    0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
    0xFF, 0x09, 0x29, 0xF6, 0x20,
    0xC0, 0x88, 0x7E, 0x09, 0x03,
    0x20, 0x54, 0x54, 0x79, 0x41,
    0x00, 0x00, 0x44, 0x7D, 0x41,
    0x30, 0x48, 0x48, 0x4A, 0x32,
    0x38, 0x40, 0x40, 0x22, 0x7A,
    0x00, 0x7A, 0x0A, 0x0A, 0x72,
    0x7D, 0x0D, 0x19, 0x31, 0x7D,
    0x26, 0x29, 0x29, 0x2F, 0x28,
    0x26, 0x29, 0x29, 0x29, 0x26,
    0x30, 0x48, 0x4D, 0x40, 0x20,
    0x38, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x38,
    0x2F, 0x10, 0xC8, 0xAC, 0xBA,
    0x2F, 0x10, 0x28, 0x34, 0xFA,
    0x00, 0x00, 0x7B, 0x00, 0x00,
    0x08, 0x14, 0x2A, 0x14, 0x22,
    0x22, 0x14, 0x2A, 0x14, 0x08,
    0xAA, 0x00, 0x55, 0x00, 0xAA,
    0xAA, 0x55, 0xAA, 0x55, 0xAA,
    0x00, 0x00, 0x00, 0xFF, 0x00,
    0x10, 0x10, 0x10, 0xFF, 0x00,
    0x14, 0x14, 0x14, 0xFF, 0x00,
    0x10, 0x10, 0xFF, 0x00, 0xFF,
    0x10, 0x10, 0xF0, 0x10, 0xF0,
    0x14, 0x14, 0x14, 0xFC, 0x00,
    0x14, 0x14, 0xF7, 0x00, 0xFF,
    0x00, 0x00, 0xFF, 0x00, 0xFF,
    0x14, 0x14, 0xF4, 0x04, 0xFC,
    0x14, 0x14, 0x17, 0x10, 0x1F,
    0x10, 0x10, 0x1F, 0x10, 0x1F,
    0x14, 0x14, 0x14, 0x1F, 0x00,
    0x10, 0x10, 0x10, 0xF0, 0x00,
    0x00, 0x00, 0x00, 0x1F, 0x10,
    0x10, 0x10, 0x10, 0x1F, 0x10,
    0x10, 0x10, 0x10, 0xF0, 0x10,
    0x00, 0x00, 0x00, 0xFF, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10,
    0x7D, 0x12, 0x11, 0x12, 0x7D,                                               // 0xC4 Ä
    0x00, 0x00, 0x00, 0xFF, 0x14,
    0x00, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0x00, 0x1F, 0x10, 0x17,
    0x00, 0x00, 0xFC, 0x04, 0xF4,
    0x14, 0x14, 0x17, 0x10, 0x17,
    0x14, 0x14, 0xF4, 0x04, 0xF4,
    0x00, 0x00, 0xFF, 0x00, 0xF7,
    0x14, 0x14, 0x14, 0x14, 0x14,
    0x14, 0x14, 0xF7, 0x00, 0xF7,
    0x14, 0x14, 0x14, 0x17, 0x14,
    0x10, 0x10, 0x1F, 0x10, 0x1F,
    0x14, 0x14, 0x14, 0xF4, 0x14,
    0x10, 0x10, 0xF0, 0x10, 0xF0,
    0x00, 0x00, 0x1F, 0x10, 0x1F,
    0x00, 0x00, 0x00, 0x1F, 0x14,
    0x00, 0x00, 0x00, 0xFC, 0x14,
    0x00, 0x00, 0xF0, 0x10, 0xF0,
    0x3D, 0x42, 0x42, 0x42, 0x3D,                                               // 0xD6 Ö
    0x14, 0x14, 0x14, 0xFF, 0x14,
    0x10, 0x10, 0x10, 0x1F, 0x00,
    0x00, 0x00, 0x00, 0xF0, 0x10,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
    0x22, 0x54, 0x54, 0x78, 0x42,                                               // 0xDC Ü (tbd)
    0x00, 0x00, 0x00, 0xFF, 0xFF,
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
    0x38, 0x44, 0x44, 0x38, 0x44,
    0x7C, 0x2A, 0x2A, 0x3E, 0x14,
    0x7E, 0x02, 0x02, 0x06, 0x06,
    0x02, 0x7E, 0x02, 0x7E, 0x02,
    0x63, 0x55, 0x49, 0x41, 0x63,
    0x22, 0x54, 0x54, 0x78, 0x42,                                               // 0xE4 ä
    0x40, 0x7E, 0x20, 0x1E, 0x20,
    0x06, 0x02, 0x7E, 0x02, 0x02,
    0x99, 0xA5, 0xE7, 0xA5, 0x99,
    0x1C, 0x2A, 0x49, 0x2A, 0x1C,
    0x4C, 0x72, 0x01, 0x72, 0x4C,
    0x30, 0x4A, 0x4D, 0x4D, 0x30,
    0x30, 0x48, 0x78, 0x48, 0x30,
    0xBC, 0x62, 0x5A, 0x46, 0x3D,
    0x3E, 0x49, 0x49, 0x49, 0x00,
    0x7E, 0x01, 0x01, 0x01, 0x7E,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x44, 0x44, 0x5F, 0x44, 0x44,
    0x40, 0x51, 0x4A, 0x44, 0x40,
    0x40, 0x44, 0x4A, 0x51, 0x40,
    0x00, 0x00, 0xFF, 0x01, 0x03,
    0xE0, 0x80, 0xFF, 0x00, 0x00,
    0x08, 0x08, 0x6B, 0x6B, 0x08,
    0x3A, 0x44, 0x44, 0x44, 0x3A,                                               // 0xF6 ö 
    0x06, 0x0F, 0x09, 0x0F, 0x06,
    0x00, 0x00, 0x18, 0x18, 0x00,
    0x00, 0x00, 0x10, 0x10, 0x00,
    0x30, 0x40, 0xFF, 0x01, 0x01,
    0x00, 0x1F, 0x01, 0x01, 0x1E,
    0x00, 0x19, 0x1D, 0x17, 0x12,                                               // 0xFC ü (tbd)
    0x00, 0x3C, 0x3C, 0x3C, 0x3C,
    0x08, 0x1C, 0x1C, 0x1C, 0x08                                                // 0xFE Energy blob
};

const unsigned char LCD_Flow [] = {
    0x00, 0x00, 0x98, 0xCC, 0x66, 0x22, 0x22, 0x22, 0xF2, 0xAA, 0x26, 0x2A, 0xF2, 0x22, 0x22, 0x22,
    0x66, 0xCC, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C,
    0x19, 0x32, 0x64, 0xC8, 0x90, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x61, 0x31, 0x18, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x08, 0x8D, 0x4A, 0xFF, 0x08, 0x08, 0x08,
    0x08, 0x18, 0x31, 0x61, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFC, 0x06, 0xF3, 0x11, 0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x05, 0x88, 0x50, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x03, 0x02, 0x02, 0x02, 0x02, 0x03, 0x00, 0x00, 0x00, 0xF0, 0x10,
    0x10, 0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x60, 0x10, 0x08, 0x04, 0x02, 0x82, 0x81, 0x81, 0x81, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x02, 0x02, 0x04, 0x84, 0x8C, 0x88, 0x88, 0x10, 0x10, 0x20, 0x40, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x07, 0x05, 0x04, 0x04, 0x07, 0x0C, 0x18, 0x30,
    0x68, 0x48, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7F, 0x40,
    0x40, 0x40, 0x42, 0x40, 0x7F, 0x40, 0x40, 0x7F, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x00, 0x06, 0x19, 0x10, 0x10, 0x1C, 0x02, 0x19, 0x24, 0x42, 0x42, 0x24, 0x19, 0x02, 0x1C, 0x10,
    0x10, 0x10, 0x10, 0x1C, 0x02, 0x19, 0x24, 0x42, 0x42, 0x24, 0x19, 0x02, 0x1C, 0x10, 0x10, 0x1F
};

unsigned int GLCDx, GLCDy;

// uses buffer
void GLCD_print_row(const far char *data)                                       // write 10 characters to LCD
{
    unsigned char x = 0;
    GLCDx = 4;

    GLCD_buffer_clr();                                                          // Clear buffer
    do {
        GLCD_write_buf2(*data++);
    } while (x++ < 9);
    GLCD_sendbuf(2);                                                            // copy buffer to LCD
}

void GLCD_print_arrows(void) {
    GLCDx = 0;
    GLCD_write_buf2('<');
    GLCDx = 9 * 12 + 8;                                                         // last character of line
    GLCD_write_buf2('>');
}


// uses buffer
void GLCD_print_menu(const far char *data, char RowAdr)                         // write string of data to LCD, with navigation arrows
{
    GLCD_buffer_clr();                                                          // Clear buffer

    if ((SubMenu && RowAdr == 4) || (!SubMenu && RowAdr == 2))                  // navigation arrows
    {
        GLCD_print_arrows();
    }

    GLCDx = 64 - (strlen(data)*6);                                              // calculate offset for centering text
    do {
        GLCD_write_buf2(*data);
    } while (*++data);

    GLCD_sendbuf(RowAdr);                                                       // copy buffer to LCD
}

/**
 * Increase or decrease int value
 * 
 * @param unsigned int Buttons
 * @param unsigned int Value
 * @param unsigned int Min
 * @param unsigned int Max
 * @return unsigned int Value
 */
unsigned int MenuNavInt(unsigned char Buttons, unsigned int Value, unsigned int Min, unsigned int Max) {
    if (Buttons == 0x3) {
        if (Value >= Max) Value = Min;
        else Value++;
    } else if (Buttons == 0x6) {
        if (Value <= Min) Value = Max;
        else Value--;
    }
   
    return Value;
}

/**
 * Get to next or previous value of an char array
 * 
 * @param unsigned char Buttons
 * @param unsigned char Value
 * @param unsigned char Count
 * @param unsigned char array Values
 * @return unsigned char Value
 */
unsigned char MenuNavCharArray(unsigned char Buttons, unsigned char Value, unsigned char Values[], unsigned char Count) {
    unsigned int i;

    for (i = 0; i < Count; i++) {
        if (Value == Values[i]) break;
    }
    i = MenuNavInt(Buttons, i, 0, Count - 1);

    return Values[i];
}

// uses buffer
void GLCDHelp(void)                                                             // Display/Scroll helptext on LCD 
{
    unsigned int x;
    
    x = strlen(MenuStr[LCDNav].Desc);
    GLCD_print_row(MenuStr[LCDNav].Desc + LCDpos);

    if (LCDpos++ == 0) ScrollTimer = Timer - 4000;
    else if (LCDpos > (x - 10)) {
        ScrollTimer = Timer - 3000;
        LCDpos = 0;
    } else ScrollTimer = Timer - 4700;
}




// called once a second

void GLCD(void) {
    unsigned char x;
    static unsigned char energy_mains = 20; // X position
    static unsigned char energy_ev = 74; // X position

    if (LCDNav) {
        if (LCDTimer++ == 120) {
            LCDNav = 0;                                                         // Exit Setup menu after 120 seconds.
            read_settings();                                                    // don't save, but restore settings
        } else return;                                                          // disable LCD status messages when navigating LCD Menu	
    }

    if (LCDTimer == 10) LCDTimer = 0;

    if (Error) {
        BACKLIGHT_ON; // LCD backlight on
        BacklightTimer = BACKLIGHT; // reset backlight timer
        
        if (Error & CT_NOCOMM) {
            GLCD_print2(0, (const far char *) "ERROR NO");
            GLCD_print2(2, (const far char *) "SERIAL COM");
            GLCD_print2(4, (const far char *) "CHECK");
            GLCD_print2(6, (const far char *) "WIRING");
            return;
        } else if (Error & TEMP_HIGH) {
            GLCD_print2(2, (const far char *) "ERROR");
            GLCD_print2(4, (const far char *) "HIGH TEMP");
        } else if (Error & RCD_TRIPPED) {
            if (LCDTimer++ < 5) {
                GLCD_print2(0, (const far char *) "RESIDUAL");
                GLCD_print2(2, (const far char *) "FAULT");
                GLCD_print2(4, (const far char *) "CURRENT");
                GLCD_print2(6, (const far char *) "DETECTED");
            } else {
                GLCD_print2(0, (const far char *) "PRESS");
                GLCD_print2(2, (const far char *) "BUTTON");
                GLCD_print2(4, (const far char *) "TO");
                GLCD_print2(6, (const far char *) "RESET");
            }
            return;
        } else if (Error & Test_IO) // Only used when testing the module
        {
            GLCD_print2(2, (const far char *) "IO Test");
            GLCD_print2(4, (const far char *) "FAILED!   ");
            GLCDx = 12 * 8 + 4;
            GLCD_write_buf2((TestState / 10) + 0x30);
            GLCD_write_buf2((TestState % 10) + 0x30);
            GLCD_sendbuf(4); // copy buffer to LCD
            return;
        }
    }

    if (TestState == 80)                                                        // Only used when testing the module
    {
        GLCD_print2(2, (const far char *) "IO Test");
        GLCD_print2(4, (const far char *) "Passed");
        return;
    }

    LCDTimer++;

                                                                                // MODE NORMAL
    if (Mode == MODE_NORMAL) {
        if (State == STATE_C) {                                                 // STATE C
            GLCD_print2(2, (const far char *) "CHARGING");

            GLCDx = 4 + 30;

            GLCD_buffer_clr(); // Clear buffer
            GLCD_write_buf2((Balanced[0] / 100) + 0x30);
            GLCD_write_buf2(((Balanced[0] / 10) % 10) + 0x30);
            GLCD_write_buf2('.');
            GLCD_write_buf2((Balanced[0] % 10) + 0x30);
            GLCD_write_buf2('A');
            GLCD_sendbuf(4); // copy buffer to LCD
        } else                                                                  
        {                                                                       // STATE A and STATE B
            glcd_clrln(0, 0x00);
            glcd_clrln(1, 0x04); // horizontal line

            if (Access_bit || Access == 0) {
                GLCD_print2(2, (const far char *) "READY TO");
                GLCD_print2(4, (const far char *) "CHARGE  ");
            } else {
                GLCD_print2(2, (const far char *) "ACCESS");
                GLCD_print2(4, (const far char *) "DENIED");
            }
            glcd_clrln(6, 0x10); // horizontal line
            glcd_clrln(7, 0x00);

        }
    }                                                                           // MODE SMART or SOLAR
    else if ((Mode == MODE_SMART) || (Mode == MODE_SOLAR)) {
        
        GLCD_Flow_buf();                                                        // copy Flow Menu to LCD buffer
          
        if (Isum < 0) {
            energy_mains -= 3;                                                  // animate the flow of Mains energy on LCD.
            if (energy_mains < 20) energy_mains = 44;                           // Only in Mode: Smart or Solar
        } else {
            energy_mains += 3;
            if (energy_mains > 44) energy_mains = 20;
        }

        GLCDx = energy_mains;
        GLCDy = 3;
                                                                                
        if (abs(Isum) >3 ) GLCD_write_buf(0xFE);                                // Show energy flow 'blob' between Grid and House
                                                                                // If current flow is < 0.3A don't show the blob

        if (State == STATE_C) {
        
            BACKLIGHT_ON;                                                       // LCD backlight on
            BacklightTimer = BACKLIGHT;
 
            energy_ev += 3; // animate energy flow to EV
            if (energy_ev > 89) energy_ev = 74;

            GLCDx = energy_ev;
            GLCDy = 3;
            GLCD_write_buf(0xFE);                                               // Show energy flow 'blob' between House and Car

            GLCDy = 2;
            GLCDx = 77;
            GLCD_write_buf((Balanced[0] / 100) + 0x30);
            GLCD_write_buf(((Balanced[0] / 10) % 10) + 0x30);
            GLCD_write_buf('A');
        }

        if (LCDTimer < 5 && Mode == MODE_SOLAR)                                 // Show Sum of currents when solar charging.
        {
            GLCDx = 26 + 3;
            GLCDy = 1;
            GLCD_write_buf(0xE3);                                               // Sum 'E' sign

            GLCDy = 2;
            if (abs(Isum) > 1000) {
                GLCDx = 14 + 3;                                                 // move first digit to left if Isum>100A
                if (Isum < 0) GLCD_write_buf('-');
                GLCDx = 20 + 3;
                GLCD_write_buf((abs((int) (Isum / 1000)) + 0x30));
            } else                                                              // Display sum of all phases (used for solar charging)
            {
                GLCDx = 20 + 3;
                if (Isum < 0) GLCD_write_buf('-');
            }
            GLCDx = 26 + 3;
            GLCD_write_buf((abs((int) (Isum / 100) % 10) + 0x30));
            GLCD_write_buf((abs((int) ((Isum) / 10) % 10) + 0x30));
            GLCD_write_buf('A');
        } else {                                                                // Displayed only in Smart and Solar modes
            for (x = 0; x < 3; x++)                                             // Display L1, L2 and L3 currents on LCD
            {
                GLCDy = x;
                GLCDx = 20 + 3;
                if (Irms[x] < 0) GLCD_write_buf('-');
                else GLCDx = 26 + 3;
                GLCD_write_buf(abs((int) (Irms[x] / 100)) + 0x30);
                GLCD_write_buf(abs((int) ((Irms[x]) / 10) % 10) + 0x30);
                GLCD_write_buf('A');
            }
        }
        GLCD_sendbuf4(0);                                                       // Copy LCD buffer to GLCD

        glcd_clrln(4, 0);                                                       // Clear line 4
        if (Error & LESS_6A) {
            if (LCDTimer < 5) {
                GLCD_print2(5, (const far char *) "WAITING");
            } else GLCD_print2(5, (const far char *) "FOR POWER");
        } else if (Error & NO_SUN) {
            if (LCDTimer < 5) {
                GLCD_print2(5, (const far char *) "WAITING");
            } else GLCD_print2(5, (const far char *) "FOR SOLAR");
        } else if (State == STATE_A || State == STATE_B) {
            GLCD_print2(5, (const far char *) "READY");                         // STATE A +B message
            if (ChargeDelay) {
                GLCDx = 12 * 8 + 4;
                GLCD_write_buf2((ChargeDelay / 10) + 0x30);
                GLCD_write_buf2((ChargeDelay % 10) + 0x30);
                GLCD_sendbuf(5); // copy buffer to LCD
            }
        } else if (State == STATE_C) {
            BACKLIGHT_ON;                                                       // LCD backlight on
            BacklightTimer = BACKLIGHT;
            
            if (LCDTimer < 7) {
                if (LCDTimer < 4 && Mode != MODE_NORMAL ) {
                    if (Mode == MODE_SOLAR) GLCD_print2(5, (const far char *) "SOLAR");
                    else GLCD_print2(5, (const far char *) "SMART");
                                        
                } else GLCD_print2(5, (const far char *) "CHARGING");
            } else {
                GLCDx = 4 + 30;

                GLCD_buffer_clr(); // Clear buffer
                GLCD_write_buf2((Balanced[0] / 100) + 0x30);
                GLCD_write_buf2(((Balanced[0] / 10) % 10) + 0x30);
                GLCD_write_buf2('.');
                GLCD_write_buf2((Balanced[0] % 10) + 0x30);
                GLCD_write_buf2('A');
            }
            GLCD_sendbuf(5); // copy buffer to LCD
        }
        glcd_clrln(7, 0x00);
    } // End Mode SMART or SOLAR

    if (BacklightTimer) BacklightTimer--;                                       // Decrease backlight counter every second.
    else BACKLIGHT_OFF;                                                         // zero? switch LCD backlight off
}


//##############################################################################################################################
// 10 CONFIG			- Set to Fixed Cable or Type 2 Socket
// 20 MODE  			- Set to Smart mode, or Normal EVSE mode
// 25       START       - Start Surplus Current (Mode=Solar)
// 27       STOP        - Stop time (Mode=Solar)
// 30		MAINS 		- Set max MAINS Current (25-100) (Mode=Smart)
// 40 MAX   			- Set MAX Charge Current for the EV (16-80)
// 50		MIN   		- Set MIN Charge Current the EV will accept (Mode=Smart)
// 60 LOCK				- Cable lock Enable/disable
// 70 CABLE				- Set Fixed Cable Current limit 
// 80 CAL 		  		- Calibrate CT1
// 90 EXIT				- Exit Menu
//100 LOADBL            - Load Balancing
//110 ACCESS            - Access control on IO2

void GLCDMenu(unsigned char Buttons) {                                          // Called when one of the SmartEVSE buttons is pressed
    static unsigned long ButtonTimer = 0;
    static unsigned char ButtonRelease = 0;                                     // keeps track of LCD Menu Navigation
    static unsigned int CT1, CT1old, value;
    static double Iold;

    unsigned char MenuItemsCount = getMenuItems();

    // Main Menu Navigation
    BacklightTimer = BACKLIGHT;                                                 // delay before LCD backlight turns off.
    BACKLIGHT_ON;                                                               // LCD backlight on	

    if (RCmon == 1 && (Error & RCD_TRIPPED) && PORTBbits.RB1 == 0)              // RCD was tripped, but RCD level is back to normal
    {
        Error &= ~RCD_TRIPPED;                                                  // Clear RCD error bit, by pressing any button
    }

    if ((LCDNav == 0) && (Buttons == 0x5) && (ButtonRelease == 0))              // Button 2 pressed ?
    {
        LCDNav = MENU_ENTER;                                                    // about to enter menu
        ButtonTimer = Timer;
    } else if (LCDNav == MENU_ENTER && ((ButtonTimer + 2000) < Timer))          // <CONFIG>
    {
        LCDNav = MENU_CONFIG;                                                   // Main Menu entered
        ButtonRelease = 1;
    } else if ((LCDNav == MENU_ENTER) && (Buttons == 0x7))                      // Button 2 released before entering menu?
    {
        LCDNav = 0;
        ButtonRelease = 0;
        GLCD();
    } else if ((LCDNav == MENU_CAL) && (Buttons == 0x2) &&  SubMenu )           // Buttons 1> and 3< pressed ?
    {                                                                           
        ICal = ICAL;                                                            // reset Calibration value    
        SubMenu = 0;                                                            // Exit Submenu
        ButtonRelease = 1;
    }    
    else if ((LCDNav > 1) && (Buttons == 0x2 || Buttons == 0x3 || Buttons == 0x6) && (ButtonRelease == 0))
    {
        if (SubMenu) {
            switch (LCDNav) {
                case MENU_CAL:
                    CT1 = MenuNavInt(Buttons, CT1, 100, 999);
                    break;
                default:
                    value = getMenuItemValue(LCDNav);
                    value = MenuNavInt(Buttons, value, MenuStr[LCDNav].Min, MenuStr[LCDNav].Max);
                    setMenuItemValue(LCDNav, value);
                    break;
            }
        } else {
            LCDNav = MenuNavCharArray(Buttons, LCDNav, MenuItems, MenuItemsCount);
        }

        ButtonRelease = 1;
    } else if (LCDNav > 1 && Buttons == 0x5 && ButtonRelease == 0)              // Button 2 pressed?
    {
        if (SubMenu)                                                            // Are we in Submenu?
        {
            SubMenu = 0;                                                        // yes, exit Submenu
            if (LCDNav == MENU_CAL)                                             // Exit CT1 calibration?
            {
                if (CT1 != CT1old)                                              // did the value change?
                {
                    Iold = (double) (CT1old / ICal);
                    ICal = (double) (CT1 / Iold);                               // Calculate new Calibration value
                    Irms[0] = CT1;                                              // Set the Irms value, so the LCD update is instant
                }
            }
        } else                                                                  // We are currently not in Submenu.
        {
            SubMenu = 1;                                                        // Enter Submenu now
            if (LCDNav == MENU_CAL)                                             // CT1 calibration start
            {
                CT1 = (unsigned int) Irms[0];                                   // make working copy of CT1 value
                CT1old = CT1;                                                   // and a backup
            } else if (LCDNav == MENU_EXIT)                                     // Exit Main Menu
            {
                LCDNav = 0;
                SubMenu = 0;
                Error = NO_ERROR;                                               // Clear All Errors when exiting the Main Menu
                TestState = 0;                                                  // Clear TestState
                ChargeDelay = 0;                                                // Clear ChargeDelay
                write_settings();                                               // Write to eeprom
                GLCD();
            }
        }
        ButtonRelease = 1;
    } else if (Buttons == 0x7)                                                  // Buttons released
    {
        ButtonRelease = 0;
        delay(10);                                                              // debounce keys
    }

    //
    // here we update the LCD
    //
    if (ButtonRelease == 1 || LCDNav == 1) {
        if (LCDNav == 1) {
            glcd_clrln(0, 0x00);
            glcd_clrln(1, 0x04);                                                // horizontal line
            GLCD_print2(2, (const far char *) "Hold 2 sec");
            GLCD_print2(4, (const far char *) "for Menu");
            glcd_clrln(6, 0x10);                                                // horizontal line
            glcd_clrln(7, 0x00);

        } else {
            GLCD_print_menu(MenuStr[LCDNav].LCD, 2);                            // add navigation arrows on both sides
            switch (LCDNav) {
                case MENU_CAL:
                    GLCD_buffer_clr();                                          // Clear buffer
                    if (SubMenu) {
                        GLCD_print_arrows();
                        GLCDx = 4 + (12 * 3);
                        GLCD_write_buf2((CT1 / 100) + 0x30);
                        GLCD_write_buf2((CT1 % 100) / 10 + 0x30);
                        GLCD_write_buf2('.');
                        GLCD_write_buf2((CT1 % 10) + 0x30);
                    } else {
                        GLCDx = 4 + (12 * 3);
                        GLCD_write_buf2(((unsigned int) Irms[0] / 100) + 0x30);
                        GLCD_write_buf2(((unsigned int) Irms[0] % 100 / 10) + 0x30);
                        GLCD_write_buf2('.');
                        GLCD_write_buf2(((unsigned int) Irms[0] % 10) + 0x30);
                    }
                    GLCDx = 4 + (12 * 7);
                    GLCD_write_buf2('A');
                    GLCD_sendbuf(4);                                            // copy buffer to LCD
                    break;
                default:
                    GLCD_print_menu(getMenuItemOption(LCDNav), 4);
                    break;
            }
        }
        ButtonRelease = 2;                                                      // Set value to 2, so that LCD will be updated only once
    }

    ScrollTimer = Timer;                                                        // reset timer for HelpMenu text
    LCDpos = 0;                                                                 // reset position of scrolling text
    OldButtonState = Buttons;
    LCDTimer = 0;
}

void st7565_command(unsigned char data) {
    _A0_0;
    PIR1bits.SSP1IF = 0;                                                        // clear flag
    SSP1BUF = data;                                                             // and send SPI data
    while (!PIR1bits.SSP1IF);                                                   // wait for bit to become set
}

void st7565_data(unsigned char data) {
    _A0_1;
    PIR1bits.SSP1IF = 0;                                                        // clear flag
    SSP1BUF = data;                                                             // and send SPI data
    while (!PIR1bits.SSP1IF);                                                   // wait for bit to become set
}

void goto_row(unsigned char y) {
    unsigned char pattern;
    pattern = 0xB0 | (y & 0xBF);                                                //put row address on data port set command     
    st7565_command(pattern);
}
//--------------------

void goto_col(unsigned char x) {
    unsigned char pattern;
    pattern = ((0xF0 & x) >> 4) | 0x10;
    st7565_command(pattern);                                                    //set high byte column command
    pattern = ((0x0F & x)) | 0x00;
    st7565_command(pattern);                                                    //set low byte column command;
}
//--------------------

void goto_xy(unsigned char x, unsigned char y) {
    goto_col(x);
    goto_row(y);
}

void glcd_clrln(unsigned char ln, unsigned char data) {
    unsigned char i;
    goto_xy(0, ln);
    for (i = 0; i < 128; i++) {
        st7565_data(data);                                                      //put data on data port  
    }
}

void GLCD_sendbuf(unsigned char RowAdr) {
    unsigned char i, x = 0;

    goto_xy(0, RowAdr);
    for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                        //put data on data port  

    goto_xy(0, RowAdr + 1);
    for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                        //put data on data port  
}

void GLCD_sendbuf4(unsigned char RowAdr) {
    unsigned int i, x = 0;

    goto_xy(0, RowAdr);
    for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                        //put data on data port

    goto_xy(0, RowAdr + 1);
    for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                        //put data on data port

    goto_xy(0, RowAdr + 2);
    for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                        //put data on data port

    goto_xy(0, RowAdr + 3);
    for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                        //put data on data port

}

void glcd_clear(void) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        glcd_clrln(i, 0);
    }
}

void GLCD_write(unsigned int c) {
    unsigned char i;
    goto_xy(GLCDx, GLCDy);
    for (i = 0; i < 5; i++) {
        st7565_data(font[(5 * c) + i]);
    }
    GLCDx = GLCDx + 6;
}

void GLCD_buffer_clr(void) {
    unsigned char x = 0;
    do {
        GLCDbuf[x++] = 0;                                                       // clear GLCD buffer
    } while (x != 0);
}

void GLCD_write_buf(unsigned int c) {
    unsigned int i, ch, x;

    x = 128 * GLCDy;
    x = x + GLCDx;
    for (i = 0; i < 5; i++) {
        ch = font[(5 * c) + i];
        GLCDbuf[x++] = ch;
    }
    GLCDx = GLCDx + 6;
}

void GLCD_write_buf2(unsigned int c) {
    unsigned char i, ch, z1, x;
    x = GLCDx;
    for (i = 0; i < 5; i++) {
        z1 = 0;
        ch = font[(5 * c) + i];
        if (ch & 0x01) z1 = z1 | 0x3;
        if (ch & 0x02) z1 = z1 | 0xc;
        if (ch & 0x04) z1 = z1 | 0x30;
        if (ch & 0x08) z1 = z1 | 0xc0;
        GLCDbuf[x++] = z1;
        GLCDbuf[x++] = z1;
    }
    x = GLCDx + 128;
    for (i = 0; i < 5; i++) {
        z1 = 0;
        ch = font[(5 * c) + i] >> 4;
        if (ch & 0x01) z1 = z1 | 0x3;
        if (ch & 0x02) z1 = z1 | 0xc;
        if (ch & 0x04) z1 = z1 | 0x30;
        if (ch & 0x08) z1 = z1 | 0xc0;
        GLCDbuf[x++] = z1;
        GLCDbuf[x++] = z1;
    }
    GLCDx = GLCDx + 12;
}

/*void GLCD_write2(unsigned int c)
{
    unsigned char i,ch,z1;
    goto_xy(GLCDx,GLCDy);
    for(i=0;i<5;i++)
    {
        z1=0;
        ch=font[(5*c)+i];
        if (ch&0x01) z1=z1|0x3;
        if (ch&0x02) z1=z1|0xc;
        if (ch&0x04) z1=z1|0x30;
        if (ch&0x08) z1=z1|0xc0;
        st7565_data(z1);    
        st7565_data(z1);    
    }
    goto_xy(GLCDx,GLCDy+1);
    for(i=0;i<5;i++)
    {
        z1=0;
        ch=font[(5*c)+i]>>4;
        if (ch&0x01) z1=z1|0x3;
        if (ch&0x02) z1=z1|0xc;
        if (ch&0x04) z1=z1|0x30;
        if (ch&0x08) z1=z1|0xc0;
        st7565_data(z1);    
        st7565_data(z1);    
    }
    GLCDx=GLCDx+12;
}
 */
//void GLCD_write3(unsigned int c)
//{
//	unsigned char i,ch,z1;
//	goto_xy(GLCDx,GLCDy);
//	for(i=0;i<5;i++)
//	{
//		z1=0;
//		ch=font[(5*c)+i];
//		if (ch&0x01) z1=z1|0x7;
//		if (ch&0x02) z1=z1|0x38;
//		if (ch&0x04) z1=z1|0xc0;
//		st7565_data(z1);    
//		st7565_data(z1);
//		if (i!=2) st7565_data(z1);
//	}
//	goto_xy(GLCDx,GLCDy+1);
//	for(i=0;i<5;i++)
//	{
//		z1=0;
//		ch=font[(5*c)+i];
//		if (ch&0x04) z1=z1|0x01;
//		if (ch&0x08) z1=z1|0x0e;
//		if (ch&0x10) z1=z1|0x70;
//		if (ch&0x20) z1=z1|0x80;
//		st7565_data(z1);    
//		st7565_data(z1);
//		if (i!=2) st7565_data(z1);
//	}
//	goto_xy(GLCDx,GLCDy+2);
//	for(i=0;i<5;i++)
//	{
//		z1=0;
//		ch=font[(5*c)+i];
//		if (ch&0x20) z1=z1|0x03;
//		if (ch&0x40) z1=z1|0x1c;
//		if (ch&0x80) z1=z1|0xe0;
//		st7565_data(z1);    
//		st7565_data(z1);
//		if (i!=2) st7565_data(z1);
//	}
//	GLCDx=GLCDx+16;
//}
//--------------------

void GLCD_print(unsigned char x, unsigned char y, const far char* str) {
    unsigned int i = 0;
    unsigned int ascii;

    GLCDx = x;
    GLCDy = y;
    for (i = 0; i < 128; i++) {
        if (str[i] == 0)
            break;
        ascii = str[i];
        GLCD_write(ascii);
    }
}

void GLCD_print2(unsigned char y, const far char* data) {
    GLCD_buffer_clr();                                                          // Clear buffer

    GLCDx = 64 - (strlen(data)*6);                                              // calculate offset for centering text
    do {
        GLCD_write_buf2(*data);
    } while (*++data);

    GLCD_sendbuf(y);                                                            // copy buffer to LCD
}

void delayus(int us) {
    while (us--) {
    };
}

void GLCD_init(void) {
    _A0_0;                                                                      // A0=0
    _RSTB_0;                                                                    // Reset GLCD module
    delayus(4);
    _RSTB_1;                                                                    // Reset line high
    delayus(4);

    st7565_command(0xA2);                                                       // set bias at duty cycle 1.65 (0xA2=1.9 0xA3=1.6)
    st7565_command(0xC8);                                                       // comm direction normal =0xC0 comm reverse= 0xC8
    st7565_command(0xA0);                                                       // seg dir (0xA0 or 0xA1)
    st7565_command(0xA6);                                                       // set inverse (0xA7=inverse 0xA6=normal)

    st7565_command(0x20 | 0x04);                                                // set Regulation Ratio (0-7)

    st7565_command(0xF8);                                                       // send Booster command
    st7565_command(0x01);                                                       // set Booster value 00=4x 01=5x

    st7565_command(0x81);                                                       // send Electronic Volume command 0x81
    st7565_command(0x24);                                                       // set Electronic volume (0x00-0x3f)

    st7565_command(0x28 | 0x07);                                                // ALL Power Control ON
    st7565_command(0x40);                                                       // Set display start line

    goto_row(0x00);                                                             // Set page address
    goto_col(0x00);                                                             // Set column addr LSB
    st7565_command(0xAF);                                                       // ON command  

}

void GLCD_version(void) {
    glcd_clear();
    GLCD_print2(2, (const far char *) "Smart EVSE");
    GLCD_print2(4, (const far char *) "Ver "VERSION);

    delay(2000);                                                                // show version for 2 seconds
}

void GLCD_Flow_buf(void) {
    unsigned int x;
    for (x = 0; x < 512; x++) GLCDbuf[x] = LCD_Flow[x];                         //copy picture data to LCD buffer
}
