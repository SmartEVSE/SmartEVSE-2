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
#include "utils.h"

// LCD font 8x5
const far unsigned char font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // 0x00
    0x3E, 0x5B, 0x4F, 0x5B, 0x3E, // 0x01
    0x3E, 0x6B, 0x4F, 0x6B, 0x3E, // 0x02
    0x1C, 0x3E, 0x7C, 0x3E, 0x1C, // 0x03
    0x18, 0x3C, 0x7E, 0x3C, 0x18, // 0x04
    0x1C, 0x57, 0x7D, 0x57, 0x1C, // 0x05
    0x1C, 0x5E, 0x7F, 0x5E, 0x1C, // 0x06
    0x00, 0x18, 0x3C, 0x18, 0x00, // 0x07
    0xFF, 0xE7, 0xC3, 0xE7, 0xFF, // 0x08
    0x00, 0x18, 0x24, 0x18, 0x00, // 0x09
    0xFF, 0xE7, 0xDB, 0xE7, 0xFF, // 0x0A
    0x30, 0x48, 0x3A, 0x06, 0x0E, // 0x0B
    0x26, 0x29, 0x79, 0x29, 0x26, // 0x0C
    0x40, 0x7F, 0x05, 0x05, 0x07, // 0x0D
    0x40, 0x7F, 0x05, 0x25, 0x3F, // 0x0E
    0x5A, 0x3C, 0xE7, 0x3C, 0x5A, // 0x0F
    0x7F, 0x3E, 0x1C, 0x1C, 0x08, // 0x10
    0x08, 0x1C, 0x1C, 0x3E, 0x7F, // 0x11
    0x14, 0x22, 0x7F, 0x22, 0x14, // 0x12
    0x5F, 0x5F, 0x00, 0x5F, 0x5F, // 0x13
    0x06, 0x09, 0x7F, 0x01, 0x7F, // 0x14
    0x00, 0x66, 0x89, 0x95, 0x6A, // 0x15
    0x60, 0x60, 0x60, 0x60, 0x60, // 0x16
    0x94, 0xA2, 0xFF, 0xA2, 0x94, // 0x17
    0x08, 0x04, 0x7E, 0x04, 0x08, // 0x18
    0x10, 0x20, 0x7E, 0x20, 0x10, // 0x19
    0x08, 0x08, 0x2A, 0x1C, 0x08, // 0x1A
    0x08, 0x1C, 0x2A, 0x08, 0x08, // 0x1B
    0x1E, 0x10, 0x10, 0x10, 0x10, // 0x1C
    0x0C, 0x1E, 0x0C, 0x1E, 0x0C, // 0x1D
    0x30, 0x38, 0x3E, 0x38, 0x30, // 0x1E
    0x06, 0x0E, 0x3E, 0x0E, 0x06, // 0x1F
    0x00, 0x00, 0x00, 0x00, 0x00, // 0x20
    0x00, 0x00, 0x5F, 0x00, 0x00, // 0x21
    0x00, 0x07, 0x00, 0x07, 0x00, // 0x22
    0x14, 0x7F, 0x14, 0x7F, 0x14, // 0x23
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // 0x24
    0x23, 0x13, 0x08, 0x64, 0x62, // 0x25
    0x36, 0x49, 0x56, 0x20, 0x50, // 0x26
    0x00, 0x08, 0x07, 0x03, 0x00, // 0x27
    0x00, 0x1C, 0x22, 0x41, 0x00, // 0x28
    0x00, 0x41, 0x22, 0x1C, 0x00, // 0x29
    0x2A, 0x1C, 0x7F, 0x1C, 0x2A, // 0x2A *
    0x08, 0x08, 0x3E, 0x08, 0x08, // 0x2B +
    0x00, 0x80, 0x70, 0x30, 0x00, // 0x2C ,
    0x08, 0x08, 0x08, 0x08, 0x08, // 0x2D -
    0x00, 0x00, 0x60, 0x60, 0x00, // 0x2E .
    0x20, 0x10, 0x08, 0x04, 0x02, // 0x2F /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0x30 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 0x31 1
    0x72, 0x49, 0x49, 0x49, 0x46, // 0x32 2
    0x21, 0x41, 0x49, 0x4D, 0x33, // 0x33 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 0x34 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 0x35 5
    0x3C, 0x4A, 0x49, 0x49, 0x31, // 0x36 6
    0x41, 0x21, 0x11, 0x09, 0x07, // 0x37 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 0x38 8
    0x46, 0x49, 0x49, 0x29, 0x1E, // 0x39 9
    0x00, 0x00, 0x14, 0x00, 0x00, // 0x3A :
    0x00, 0x40, 0x34, 0x00, 0x00, // 0x3B ;
    0x00, 0x08, 0x14, 0x22, 0x41, // 0x3C <
    0x14, 0x14, 0x14, 0x14, 0x14, // 0x3D =
    0x00, 0x41, 0x22, 0x14, 0x08, // 0x3E >
    0x02, 0x01, 0x59, 0x09, 0x06, // 0x3F ?
    0x3E, 0x41, 0x5D, 0x59, 0x4E, // 0x40 @
    0x7C, 0x12, 0x11, 0x12, 0x7C, // 0x41 A
    0x7F, 0x49, 0x49, 0x49, 0x36, // 0x42 B
    0x3E, 0x41, 0x41, 0x41, 0x22, // 0x43 C
    0x7F, 0x41, 0x41, 0x41, 0x3E, // 0x44 D
    0x7F, 0x49, 0x49, 0x49, 0x41, // 0x45 E
    0x7F, 0x09, 0x09, 0x09, 0x01, // 0x46 F
    0x3E, 0x41, 0x41, 0x51, 0x73, // 0x47 G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // 0x48 H
    0x00, 0x41, 0x7F, 0x41, 0x00, // 0x49 I
    0x20, 0x40, 0x41, 0x3F, 0x01, // 0x4A J
    0x7F, 0x08, 0x14, 0x22, 0x41, // 0x4B K
    0x7F, 0x40, 0x40, 0x40, 0x40, // 0x4C L
    0x7F, 0x02, 0x1C, 0x02, 0x7F, // 0x4D M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // 0x4E N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // 0x4F O
    0x7F, 0x09, 0x09, 0x09, 0x06, // 0x50 P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // 0x51 Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // 0x52 R
    0x26, 0x49, 0x49, 0x49, 0x32, // 0x53 S
    0x03, 0x01, 0x7F, 0x01, 0x03, // 0x54 T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // 0x55 U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // 0x56 V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // 0x57 W
    0x63, 0x14, 0x08, 0x14, 0x63, // 0x58 X
    0x03, 0x04, 0x78, 0x04, 0x03, // 0x59 Y
    0x61, 0x59, 0x49, 0x4D, 0x43, // 0x5A Z
    0x00, 0x7F, 0x41, 0x41, 0x41, // 0x5B [
    0x02, 0x04, 0x08, 0x10, 0x20, // 0x5C
    0x00, 0x41, 0x41, 0x41, 0x7F, // 0x5D ]
    0x04, 0x02, 0x01, 0x02, 0x04, // 0x5E ^
    0x40, 0x40, 0x40, 0x40, 0x40, // 0x5F _
    0x00, 0x03, 0x07, 0x08, 0x00, // 0x60 '
    0x20, 0x54, 0x54, 0x78, 0x40, // 0x61 a
    0x7F, 0x28, 0x44, 0x44, 0x38, // 0x62 b
    0x38, 0x44, 0x44, 0x44, 0x28, // 0x63 c
    0x38, 0x44, 0x44, 0x28, 0x7F, // 0x64 d
    0x38, 0x54, 0x54, 0x54, 0x18, // 0x65 e
    0x00, 0x08, 0x7E, 0x09, 0x02, // 0x66 f
    0x18, 0xA4, 0xA4, 0x9C, 0x78, // 0x67 g
    0x7F, 0x08, 0x04, 0x04, 0x78, // 0x68 h
    0x00, 0x44, 0x7D, 0x40, 0x00, // 0x69 i
    0x20, 0x40, 0x40, 0x3D, 0x00, // 0x6A j
    0x7F, 0x10, 0x28, 0x44, 0x00, // 0x6B k
    0x00, 0x41, 0x7F, 0x40, 0x00, // 0x6C l
    0x7C, 0x04, 0x78, 0x04, 0x78, // 0x6D m
    0x7C, 0x08, 0x04, 0x04, 0x78, // 0x6E n
    0x38, 0x44, 0x44, 0x44, 0x38, // 0x6F o
    0xFC, 0x18, 0x24, 0x24, 0x18, // 0x70 p
    0x18, 0x24, 0x24, 0x18, 0xFC, // 0x71 q
    0x7C, 0x08, 0x04, 0x04, 0x08, // 0x72 r
    0x48, 0x54, 0x54, 0x54, 0x24, // 0x73 s
    0x04, 0x04, 0x3F, 0x44, 0x24, // 0x74 t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // 0x75 u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // 0x76 v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // 0x77 w
    0x44, 0x28, 0x10, 0x28, 0x44, // 0x78 x
    0x4C, 0x90, 0x90, 0x90, 0x7C, // 0x79 y
    0x44, 0x64, 0x54, 0x4C, 0x44, // 0x7A z
    0x00, 0x08, 0x36, 0x41, 0x00, // 0x7B (
    0x00, 0x00, 0x77, 0x00, 0x00, // 0x7C
    0x00, 0x41, 0x36, 0x08, 0x00, // 0x7D )
    0x02, 0x01, 0x02, 0x04, 0x02, // 0x7E ~
    0x3C, 0x26, 0x23, 0x26, 0x3C, // 0x7F
    0x1E, 0xA1, 0xA1, 0x61, 0x12, // 0x80
    0x3A, 0x40, 0x40, 0x20, 0x7A, // 0x81 ü
    0x38, 0x54, 0x54, 0x55, 0x59, // 0x82 é
    0x21, 0x55, 0x55, 0x79, 0x41, // 0x83 â
    0x21, 0x54, 0x54, 0x78, 0x41, // 0x84 ä
    0x21, 0x55, 0x54, 0x78, 0x40, // 0x85 à
    0x20, 0x54, 0x55, 0x79, 0x40, // 0x86 á
    0x0C, 0x1E, 0x52, 0x72, 0x12, // 0x87
    0x39, 0x55, 0x55, 0x55, 0x59, // 0x88 ê
    0x39, 0x54, 0x54, 0x54, 0x59, // 0x89
    0x39, 0x55, 0x54, 0x54, 0x58, // 0x8A è
    0x00, 0x00, 0x45, 0x7C, 0x41, // 0x8B
    0x00, 0x02, 0x45, 0x7D, 0x42, // 0x8C
    0x00, 0x01, 0x45, 0x7C, 0x40, // 0x8D ì
    0xF0, 0x29, 0x24, 0x29, 0xF0, // 0x8E Ä
    0xF0, 0x28, 0x25, 0x28, 0xF0, // 0x8F
    0x7C, 0x54, 0x55, 0x45, 0x00, // 0x90 É
    0x20, 0x54, 0x54, 0x7C, 0x54, // 0x91
    0x7C, 0x0A, 0x09, 0x7F, 0x49, // 0x92
    0x32, 0x49, 0x49, 0x49, 0x32, // 0x93 ô
    0x32, 0x48, 0x48, 0x48, 0x32, // 0x94 ö
    0x32, 0x4A, 0x48, 0x48, 0x30, // 0x95 ò
    0x3A, 0x41, 0x41, 0x21, 0x7A, // 0x96 û
    0x3A, 0x42, 0x40, 0x20, 0x78, // 0x97 ù
    0x00, 0x9D, 0xA0, 0xA0, 0x7D, // 0x98
    0x39, 0x44, 0x44, 0x44, 0x39, // 0x99 Ö
    0x3D, 0x40, 0x40, 0x40, 0x3D, // 0x9A Ü
    0x3C, 0x24, 0xFF, 0x24, 0x24, // 0x9B ¢
    0x48, 0x7E, 0x49, 0x43, 0x66, // 0x9C
    0x2B, 0x2F, 0xFC, 0x2F, 0x2B, // 0x9D
    0xFF, 0x09, 0x29, 0xF6, 0x20, // 0x9E
    0xC0, 0x88, 0x7E, 0x09, 0x03, // 0x9F
    0x20, 0x54, 0x54, 0x79, 0x41, // 0xA0
    0x00, 0x00, 0x44, 0x7D, 0x41, // 0xA1
    0x30, 0x48, 0x48, 0x4A, 0x32, // 0xA2
    0x38, 0x40, 0x40, 0x22, 0x7A, // 0xA3
    0x00, 0x7A, 0x0A, 0x0A, 0x72, // 0xA4
    0x7D, 0x0D, 0x19, 0x31, 0x7D, // 0xA5
    0x26, 0x29, 0x29, 0x2F, 0x28, // 0xA6
    0x26, 0x29, 0x29, 0x29, 0x26, // 0xA7
    0x30, 0x48, 0x4D, 0x40, 0x20, // 0xA8
    0x38, 0x08, 0x08, 0x08, 0x08, // 0xA9
    0x08, 0x08, 0x08, 0x08, 0x38, // 0xAA
    0x2F, 0x10, 0xC8, 0xAC, 0xBA, // 0xAB
    0x2F, 0x10, 0x28, 0x34, 0xFA, // 0xAC
    0x00, 0x00, 0x7B, 0x00, 0x00, // 0xAD
    0x08, 0x14, 0x2A, 0x14, 0x22, // 0xAE
    0x22, 0x14, 0x2A, 0x14, 0x08, // 0xAF
    0xAA, 0x00, 0x55, 0x00, 0xAA, // 0xB0
    0xAA, 0x55, 0xAA, 0x55, 0xAA, // 0xB1
    0x00, 0x00, 0x00, 0xFF, 0x00, // 0xB2
    0x10, 0x10, 0x10, 0xFF, 0x00, // 0xB3
    0x14, 0x14, 0x14, 0xFF, 0x00, // 0xB4
    0x10, 0x10, 0xFF, 0x00, 0xFF, // 0xB5
    0x10, 0x10, 0xF0, 0x10, 0xF0, // 0xB6
    0x14, 0x14, 0x14, 0xFC, 0x00, // 0xB7
    0x14, 0x14, 0xF7, 0x00, 0xFF, // 0xB8
    0x00, 0x00, 0xFF, 0x00, 0xFF, // 0xB9
    0x14, 0x14, 0xF4, 0x04, 0xFC, // 0xBA
    0x14, 0x14, 0x17, 0x10, 0x1F, // 0xBB
    0x10, 0x10, 0x1F, 0x10, 0x1F, // 0xBC
    0x14, 0x14, 0x14, 0x1F, 0x00, // 0xBD
    0x10, 0x10, 0x10, 0xF0, 0x00, // 0xBE
    0x00, 0x00, 0x00, 0x1F, 0x10, // 0xBF
    0x10, 0x10, 0x10, 0x1F, 0x10, // 0xC0
    0x10, 0x10, 0x10, 0xF0, 0x10, // 0xC1
    0x00, 0x00, 0x00, 0xFF, 0x10, // 0xC2
    0x10, 0x10, 0x10, 0x10, 0x10, // 0xC3
    0x7D, 0x12, 0x11, 0x12, 0x7D, // 0xC4 Ä
    0x00, 0x00, 0x00, 0xFF, 0x14, // 0xC5
    0x00, 0x00, 0xFF, 0x00, 0xFF, // 0xC6
    0x00, 0x00, 0x1F, 0x10, 0x17, // 0xC7
    0x00, 0x00, 0xFC, 0x04, 0xF4, // 0xC8
    0x14, 0x14, 0x17, 0x10, 0x17, // 0xC9
    0x14, 0x14, 0xF4, 0x04, 0xF4, // 0xCA
    0x00, 0x00, 0xFF, 0x00, 0xF7, // 0xCB
    0x14, 0x14, 0x14, 0x14, 0x14, // 0xCC
    0x14, 0x14, 0xF7, 0x00, 0xF7, // 0xCD
    0x14, 0x14, 0x14, 0x17, 0x14, // 0xCE
    0x10, 0x10, 0x1F, 0x10, 0x1F, // 0xCF
    0x14, 0x14, 0x14, 0xF4, 0x14, // 0xD0
    0x10, 0x10, 0xF0, 0x10, 0xF0, // 0xD1
    0x00, 0x00, 0x1F, 0x10, 0x1F, // 0xD2
    0x00, 0x00, 0x00, 0x1F, 0x14, // 0xD3
    0x00, 0x00, 0x00, 0xFC, 0x14, // 0xD4
    0x00, 0x00, 0xF0, 0x10, 0xF0, // 0xD5
    0x3D, 0x42, 0x42, 0x42, 0x3D, // 0xD6 Ö
    0x14, 0x14, 0x14, 0xFF, 0x14, // 0xD7
    0x10, 0x10, 0x10, 0x1F, 0x00, // 0xD8
    0x00, 0x00, 0x00, 0xF0, 0x10, // 0xD9
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 0xDA
    0xF0, 0xF0, 0xF0, 0xF0, 0xF0, // 0xDB
    0x22, 0x54, 0x54, 0x78, 0x42, // 0xDC Ü (tbd)
    0x00, 0x00, 0x00, 0xFF, 0xFF, // 0xDD
    0x0F, 0x0F, 0x0F, 0x0F, 0x0F, // 0xDE
    0x38, 0x44, 0x44, 0x38, 0x44, // 0xDF
    0x7C, 0x2A, 0x2A, 0x3E, 0x14, // 0xE0
    0x7E, 0x02, 0x02, 0x06, 0x06, // 0xE1
    0x02, 0x7E, 0x02, 0x7E, 0x02, // 0xE2
    0x63, 0x55, 0x49, 0x41, 0x63, // 0xE3
    0x22, 0x54, 0x54, 0x78, 0x42, // 0xE4 ä
    0x40, 0x7E, 0x20, 0x1E, 0x20, // 0xE5
    0x06, 0x02, 0x7E, 0x02, 0x02, // 0xE6
    0x99, 0xA5, 0xE7, 0xA5, 0x99, // 0xE7
    0x1C, 0x2A, 0x49, 0x2A, 0x1C, // 0xE8
    0x4C, 0x72, 0x01, 0x72, 0x4C, // 0xE9
    0x30, 0x4A, 0x4D, 0x4D, 0x30, // 0xEA
    0x30, 0x48, 0x78, 0x48, 0x30, // 0xEB
    0xBC, 0x62, 0x5A, 0x46, 0x3D, // 0xEC
    0x3E, 0x49, 0x49, 0x49, 0x00, // 0xED
    0x7E, 0x01, 0x01, 0x01, 0x7E, // 0xEE
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, // 0xEF
    0x44, 0x44, 0x5F, 0x44, 0x44, // 0xF0
    0x40, 0x51, 0x4A, 0x44, 0x40, // 0xF1
    0x40, 0x44, 0x4A, 0x51, 0x40, // 0xF2
    0x00, 0x00, 0xFF, 0x01, 0x03, // 0xF3
    0xE0, 0x80, 0xFF, 0x00, 0x00, // 0xF4
    0x08, 0x08, 0x6B, 0x6B, 0x08, // 0xF5
    0x3A, 0x44, 0x44, 0x44, 0x3A, // 0xF6 ö
    0x06, 0x0F, 0x09, 0x0F, 0x06, // 0xF7
    0x00, 0x00, 0x03, 0x03, 0x00, // 0xF8 °
    0x00, 0x00, 0x10, 0x10, 0x00, // 0xF9
    0x30, 0x40, 0xFF, 0x01, 0x01, // 0xFA
    0x00, 0x1F, 0x01, 0x01, 0x1E, // 0xFB
    0x00, 0x19, 0x1D, 0x17, 0x12, // 0xFC ü (tbd)
    0x00, 0x3C, 0x3C, 0x3C, 0x3C, // 0xFD
    0x08, 0x1C, 0x1C, 0x1C, 0x08  // 0xFE Energy blob
};

const unsigned char LCD_Flow [] = {
0x00, 0x00, 0x98, 0xCC, 0x66, 0x22, 0x22, 0x22, 0xF2, 0xAA, 0x26, 0x2A, 0xF2, 0x22, 0x22, 0x22,
0x66, 0xCC, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x60, 0xC0,
0x90, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x42, 0x04, 0xE0, 0x10, 0x08,
0x0B, 0x08, 0x10, 0xE0, 0x04, 0x42, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x4F,
0x49, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x40, 0x61, 0x31, 0x18, 0x08, 0x08, 0x08, 0x08, 0xFF, 0x08, 0x8D, 0x4A, 0xFF, 0x08, 0x08, 0x08,
0x08, 0x18, 0x31, 0x61, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x03, 0x06, 0x0C, 0x19, 0x32, 0x64, 0xC8, 0x10, 0x00, 0x00, 0x08, 0x04, 0x00, 0x01, 0x02,
0x1A, 0x02, 0x01, 0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x05, 0x88, 0x50, 0xFF, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0x00, 0xF8, 0x08, 0x08, 0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, 0xF0, 0x10,
0x10, 0x10, 0x10, 0x10, 0xF0, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0x60, 0x10, 0x08, 0x04, 0x02, 0x82, 0x81, 0x81, 0x81, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x02, 0x02, 0x04, 0x84, 0x8C, 0x88, 0x88, 0x10, 0x10, 0x20, 0x40, 0x80, 0x00,
0x00, 0x00, 0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x07, 0x05, 0x04, 0x04, 0x07, 0x0C, 0x18, 0x30,
0x68, 0x48, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x00, 0x7F, 0x40, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x40, 0x40, 0x40, 0x7F, 0x40,
0x40, 0x40, 0x42, 0x40, 0x7F, 0x40, 0x40, 0x7F, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
0x00, 0x06, 0x19, 0x10, 0x10, 0x1C, 0x02, 0x19, 0x24, 0x42, 0x42, 0x24, 0x19, 0x02, 0x1C, 0x10,
0x10, 0x10, 0x10, 0x1C, 0x02, 0x19, 0x24, 0x42, 0x42, 0x24, 0x19, 0x02, 0x1C, 0x10, 0x10, 0x1F
};

unsigned int GLCDx, GLCDy;

// uses buffer
void GLCD_print_row(const far char *data)                                       // write 10+ characters to LCD
{
    GLCD_buffer_clr();                                                          // Clear buffer
    GLCDx = 2;

    do {
        GLCD_write_buf2(*data++);
    } while (GLCDx < 118);
    GLCD_sendbuf2(2);                                                           // copy buffer to LCD
}

void GLCD_print_buf2_noclr(unsigned char y, const far char* str) {
    unsigned char i = 0, s, e;

    GLCDx = 65;
    while (str[i]) {                                                            // calculate offset for centering text
        s = 0;
        e = 5;
        font_condense(str[i], &s, &e, 0);
        GLCDx -= (e - s) + 1;
        i++;
    }
    i=0;
    while (str[i]) {
        GLCD_write_buf2(str[i++]);
    }
    GLCD_sendbuf2(y);                                                            // copy buffer to LCD
}


// Write Menu to buffer, then send to GLCD
void GLCD_print_menu(unsigned char y, const far char* str) {
    GLCD_buffer_clr();                                                          // Clear buffer

    if ((SubMenu && y == 4) || (!SubMenu && y == 2)) {                          // navigation arrows
        GLCDx = 0;
        GLCD_write_buf2('<');
        GLCDx = 9 * 12 + 8;                                                     // last character of line
        GLCD_write_buf2('>');
    }
    GLCD_print_buf2_noclr(y, str);
}


void GLCD_print_buf2(unsigned char y, const far char* str) {
    GLCD_buffer_clr();                                                          // Clear buffer
    GLCD_print_buf2_noclr(y, str);
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
    i = MenuNavInt(Buttons, i, 0, Count - 1u);

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
    unsigned int seconds, minutes;
    static unsigned char energy_mains = 20; // X position
    static unsigned char energy_ev = 74; // X position
    unsigned char Str[10];

    LCDTimer++;

    if (LCDNav) {
        if (LCDNav == MENU_RFIDREADER && SubMenu) {
            if (RFIDstatus == 2) GLCD_print(0,0, (const far char*) "Card Stored");
            else if (RFIDstatus == 3) GLCD_print(0,0, (const far char*) "Card Deleted");
            else if (RFIDstatus == 4) GLCD_print(0,0, (const far char*) "Card already stored!");
            else if (RFIDstatus == 5) GLCD_print(0,0, (const far char*) "Card not in storage!");
            else if (RFIDstatus == 6) GLCD_print(0,0, (const far char*) "Card storage full!");
            else glcd_clrln(0, 0x00);                                           // Clear line
            LCDTimer = 0;                                                       // reset timer, so it will not exit the menu when learning/deleting cards
        }

        if (LCDTimer == 120) {
            LCDNav = 0;                                                         // Exit Setup menu after 120 seconds.
            read_settings();                                                    // don't save, but restore settings
        } else return;                                                          // disable LCD status messages when navigating LCD Menu	
    }

    if (LCDTimer == 12) LCDTimer = 0;

    if (Error) {
        BACKLIGHT_ON;                                                           // We switch backlight on, as we exit after displaying the error
        BacklightTimer = BACKLIGHT;                                             // Backlight timer is set to 60 seconds
        
        if (Error & CT_NOCOMM) {                                                // No serial communication for 10 seconds
            GLCD_print_buf2(0, (const far char *) "ERROR NO");
            GLCD_print_buf2(2, (const far char *) "SERIAL COM");
            GLCD_print_buf2(4, (const far char *) "CHECK");
            GLCD_print_buf2(6, (const far char *) "WIRING");
            return;
        } else if (Error & TEMP_HIGH) {                                         // Temperature reached 65C
            GLCD_print_buf2(0, (const far char *) "HIGH TEMP");
            GLCD_print_buf2(2, (const far char *) "ERROR");
            GLCD_print_buf2(4, (const far char *) "CHARGING");
            GLCD_print_buf2(6, (const far char *) "STOPPED");
            return;
        } else if (Error & RCD_TRIPPED) {                                       // RCD sensor tripped
            if (LCDTimer < 6) {
                GLCD_print_buf2(0, (const far char *) "RESIDUAL");
                GLCD_print_buf2(2, (const far char *) "FAULT");
                GLCD_print_buf2(4, (const far char *) "CURRENT");
                GLCD_print_buf2(6, (const far char *) "DETECTED");
            } else {
                GLCD_print_buf2(0, (const far char *) "PRESS");
                GLCD_print_buf2(2, (const far char *) "BUTTON");
                GLCD_print_buf2(4, (const far char *) "TO");
                GLCD_print_buf2(6, (const far char *) "RESET");
            }
            return;
        } else if (Error & Test_IO) {                                           // Only used when testing the module
            GLCD_print_buf2(2, (const far char *) "IO Test");
            sprintf(Str, "FAILED! %u", TestState);
            GLCD_print_buf2(4, Str);
            return;
        } else if (Error & BL_FLASH) {                                          // Bootloader update error
            GLCD_print_buf2(2, (const far char *) "BOOTLOADER");
            GLCD_print_buf2(4, (const far char *) "UPDATE ERR");
            return;
        }
    }   // end of ERROR()                                                       // more specific error handling in the code below

    if (TestState == 80) {                                                      // Only used when testing the module
        GLCD_print_buf2(2, (const far char *) "IO Test");
        GLCD_print_buf2(4, (const far char *) "Passed");
        return;
    }

                                                                                // MODE NORMAL
    if (Mode == MODE_NORMAL || !Access_bit) {

        glcd_clrln(0, 0x00);
        glcd_clrln(1, 0x04);                                                    // horizontal line
        glcd_clrln(6, 0x10);                                                    // horizontal line
        glcd_clrln(7, 0x00);

        if (Error & LESS_6A) {
            GLCD_print_buf2(2, (const far char *) "WAITING");
            GLCD_print_buf2(4, (const far char *) "FOR POWER");
        } else if (State == STATE_C) {                                          // STATE C
            
            BacklightTimer = BACKLIGHT;
            
            GLCD_print_buf2(2, (const far char *) "CHARGING");
            sprintf(Str, "%u.%uA",Balanced[0] / 10, Balanced[0] % 10);
            GLCD_print_buf2(4, Str);
        } else {                                                                // STATE A and STATE B
            if (Access_bit) {
                GLCD_print_buf2(2, (const far char *) "READY TO");
                sprintf(Str, "CHARGE %u", ChargeDelay);
                if (ChargeDelay) {
                    BacklightTimer = BACKLIGHT;
                } else Str[6] = '\0';
                GLCD_print_buf2(4, Str);
            } else {
                if (RFIDReader) {
                    if (RFIDstatus == 7) {
                        GLCD_print_buf2(2, (const far char *) "INVALID");
                        GLCD_print_buf2(4, (const far char *) "RFID CARD");
                    } else {
                        GLCD_print_buf2(2, (const far char *) "PRESENT");
                        GLCD_print_buf2(4, (const far char *) "RFID CARD");
                    }
                } else {
                    GLCD_print_buf2(2, (const far char *) "ACCESS");
                    GLCD_print_buf2(4, (const far char *) "DENIED");
                }
            }
        }
    }                                                                           // MODE SMART or SOLAR
    else if ((Mode == MODE_SMART) || (Mode == MODE_SOLAR)) {
        
        memcpy (GLCDbuf, LCD_Flow, 512);                                        // copy Flow Menu to LCD buffer

        if (Mode == MODE_SMART) {                                               // remove the Sun from the LCD buffer
            for (x=0; x<13; x++) {
                GLCDbuf[x+74u] = 0;
                GLCDbuf[x+74u+128u] = 0;
            }
        }
        if (!SolarTimerEnable) {                                                // remove the clock from the LCD buffer
            for (x=0; x<8; x++) GLCDbuf[x+92u] = 0;
        } else {                                                                // display remaining time before charging is stopped
            seconds = StopTime * 60;
            seconds = seconds - SolarStopTimer;
            minutes = seconds / 60;
            seconds = seconds % 60;
            sprintf(Str, "%02u:%02u", minutes, seconds);
            GLCD_print_buf(100, 0, Str);                                        // print to buffer
        }
        
        
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

        if (EVMeter) {                                                          // If we have a EV kWh meter configured, Show total charged energy in kWh on LCD.
            sprintfd(Str, "%2u.%1ukWh", EnergyCharged, 1);                      // Will reset to 0.0kWh when charging cable reconnected, and state change from STATE B->C
            GLCD_print_buf(88, 1, Str);                                         // print to buffer
        }

        if (State == STATE_C) {
            BacklightTimer = BACKLIGHT;

            energy_ev += 3;                                                     // animate energy flow to EV
            if (energy_ev > 89) energy_ev = 74;

            GLCDx = energy_ev;
            GLCDy = 3;
            GLCD_write_buf(0xFE);                                               // Show energy flow 'blob' between House and Car

            sprintfd(Str, "%2uA", Balanced[0] / 10.0, 0);
            GLCD_print_buf(77, 2, Str);                                         // print to buffer
        }

        if (LCDTimer < 5 && Mode == MODE_SOLAR) {                               // Show Sum of currents when solar charging.
            GLCDx = 41;
            GLCDy = 1;
            GLCD_write_buf(0xE3);                                               // Sum 'E' sign

            sprintfd(Str, "%3dA", Isum / 10.0, 0);
            GLCD_print_buf(23, 2, Str);                                         // print to buffer

        } else {                                                                // Displayed only in Smart and Solar modes
            for (x = 0; x < 3; x++) {                                           // Display L1, L2 and L3 currents on LCD
                sprintfd(Str, "%3dA", Irms[x] / 10.0, 0);
                GLCD_print_buf(23, x, Str);                                     // print to buffer
            }
        }
        GLCD_sendbuf4(0);                                                       // Copy LCD buffer to GLCD

        glcd_clrln(4, 0);                                                       // Clear line 4
        if (Error & LESS_6A) {
            if (LCDTimer < 6) {
                GLCD_print_buf2(5, (const far char *) "WAITING");
            } else GLCD_print_buf2(5, (const far char *) "FOR POWER");
        } else if (Error & NO_SUN) {
            if (LCDTimer < 6) {
                GLCD_print_buf2(5, (const far char *) "WAITING");
            } else GLCD_print_buf2(5, (const far char *) "FOR SOLAR");
        } else if (State != STATE_C) { 
            sprintf(Str, "READY %u", ChargeDelay);
            if (ChargeDelay) {
                BacklightTimer = BACKLIGHT;
            } else Str[5] = '\0';
            GLCD_print_buf2(5, Str);
        } else if (State == STATE_C) {

            if (LCDTimer < 7) {                                                 //LCDtimer 0-5 sec
                if (LCDTimer < 4 && Mode != MODE_NORMAL ) {
                    if (Mode == MODE_SOLAR) GLCD_print_buf2(5, (const far char *) "SOLAR");
                    else GLCD_print_buf2(5, (const far char *) "SMART");

                } else GLCD_print_buf2(5, (const far char *) "CHARGING");
            } else {                                                            //LCDTimer 6-9 sec
                if (EVMeter && LCDTimer > 8) sprintfd(Str, "%u.%02u kWh", EnergyCharged, 2);
                else sprintf(Str, "%u.%uA", Balanced[0] / 10, Balanced[0] % 10);
                GLCD_print_buf2(5, Str);
            }
        }
        glcd_clrln(7, 0x00);
    } // End Mode SMART or SOLAR

    if (BacklightTimer){
        BACKLIGHT_ON;
        BacklightTimer--;                                                       // Decrease backlight counter every second.
    } else BACKLIGHT_OFF;                                                       // zero? switch LCD backlight off
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
    unsigned char Str[10];

    unsigned char MenuItemsCount = getMenuItems();

    // Main Menu Navigation
    BacklightTimer = BACKLIGHT;                                                 // delay before LCD backlight turns off.
    BACKLIGHT_ON;                                                               // LCD backlight on	

    if (RCmon == 1 && (Error & RCD_TRIPPED) && PORTBbits.RB1 == 0) {            // RCD was tripped, but RCD level is back to normal
        Error &= ~RCD_TRIPPED;                                                  // Clear RCD error bit, by pressing any button
    }
        
    if ((LCDNav == 0) && (Buttons == 0x5) && (ButtonRelease == 0)) {            // Button 2 pressed ?
        LCDNav = MENU_ENTER;                                                    // about to enter menu
        ButtonTimer = Timer;
    } else if (LCDNav == MENU_ENTER && ((ButtonTimer + 2000) < Timer)) {        // <CONFIG>
        LCDNav = MENU_CONFIG;                                                   // Main Menu entered
        ButtonRelease = 1;
    } else if ((LCDNav == MENU_ENTER) && (Buttons == 0x7)) {                    // Button 2 released before entering menu?
        LCDNav = 0;
        ButtonRelease = 0;
        GLCD();
    } else if ((LCDNav == MENU_CAL) && (Buttons == 0x2) &&  SubMenu ) {         // Buttons 1> and 3< pressed ?
        ICal = ICAL;                                                            // reset Calibration value    
        SubMenu = 0;                                                            // Exit Submenu
        ButtonRelease = 1;
    } else if ((LCDNav > 1) && (Buttons == 0x2 ||                               // Buttons < or > or both pressed
            Buttons == 0x3 || Buttons == 0x6) && (ButtonRelease == 0)) {        // We are navigating between sub menu options
        if (SubMenu) {
            switch (LCDNav) {
                case MENU_CAL:
                    CT1 = MenuNavInt(Buttons, CT1, 100, 999);
                    break;
                case MENU_EVMETER:                                              // do not display the Sensorbox and custom meter here
                    value = getItemValue(LCDNav);
                    do {
                        value = MenuNavInt(Buttons, value, MenuStr[LCDNav].Min, MenuStr[LCDNav].Max);
                    } while (EMConfig[value].ERegister == 0xFFFF);
                    setItemValue(LCDNav, value);
                    break;
                default:
                    value = getItemValue(LCDNav);
                    value = MenuNavInt(Buttons, value, MenuStr[LCDNav].Min, MenuStr[LCDNav].Max);
                    setItemValue(LCDNav, value);
                    break;
            }
        } else {
            LCDNav = MenuNavCharArray(Buttons, LCDNav, MenuItems, MenuItemsCount);
        }
        ButtonRelease = 1;
    } else if (LCDNav > 1 && Buttons == 0x5 && ButtonRelease == 0) {            // Button 2 pressed?
        ButtonRelease = 1;
        if (SubMenu) {                                                          // We are currently in Submenu
            SubMenu = 0;                                                        // Exit Submenu now
            if (LCDNav == MENU_CAL) {                                           // Exit CT1 calibration?
                if (CT1 != CT1old) {                                            // did the value change?
                    Iold = (double) (CT1old / ICal);
                    ICal = (double) (CT1 / Iold);                               // Calculate new Calibration value
                    Irms[0] = CT1;                                              // Set the Irms value, so the LCD update is instant
                }
            }
        } else {                                                                // We are currently not in Submenu.
            SubMenu = 1;                                                        // Enter Submenu now
            if (LCDNav == MENU_CAL) {                                           // CT1 calibration start
                CT1 = (unsigned int) abs(Irms[0]);                              // make working copy of CT1 value
                CT1old = CT1;                                                   // and a backup
            } else if (LCDNav == MENU_EXIT) {                                   // Exit Main Menu
                LCDNav = 0;
                SubMenu = 0;
                Error = NO_ERROR;                                               // Clear All Errors when exiting the Main Menu
                TestState = 0;                                                  // Clear TestState
                ChargeDelay = 0;                                                // Clear ChargeDelay
                SolarTimerEnable = 0;                                           // Disable Solar Timer
                GLCD();                                             
                write_settings();                                               // Write to eeprom
                ButtonRelease = 2;                                              // Skip updating of the LCD 
            }
        }
        
    } else if (Buttons == 0x7) {                                                // Buttons released
        ButtonRelease = 0;
        delay(10);                                                              // debounce keys (blocking)
    }

    //
    // here we update the LCD
    //
    if ( ButtonRelease == 1 || LCDNav == 1) {
        if (LCDNav == 1) {
            glcd_clrln(0, 0x00);
            glcd_clrln(1, 0x04);                                                // horizontal line
            GLCD_print_buf2(2, (const far char *) "Hold 2 sec");
            GLCD_print_buf2(4, (const far char *) "for Menu");
            glcd_clrln(6, 0x10);                                                // horizontal line
            glcd_clrln(7, 0x00);

        } else {
            GLCD_print_menu(2, MenuStr[LCDNav].LCD);                            // add navigation arrows on both sides
            switch (LCDNav) {
                case MENU_CAL:
                    if (SubMenu) {
                        sprintf(Str, "%u.%uA",CT1 / 10, CT1 % 10);
                    } else {
                        sprintf(Str, "%u.%uA",((unsigned int) abs(Irms[0]) / 10), ((unsigned int) abs(Irms[0]) % 10) );
                    }
                    GLCD_print_menu(4, Str);
                    break;
                default:
                    GLCD_print_menu(4, getMenuItemOption(LCDNav));              // print Menu
                    break;
            }

            // Bottom row of the GLCD
            glcd_clrln(7, 0x00);                                                // Clear line
            sprintf(Str, "%3i\370C", TempEVSE);                                 // \370 is the octal representation of the ° symbol
            GLCD_print(0,7, Str);                                               // show the internal temperature
            sprintf(Str, "%2u/%u", GetPosInMenu(MenuItemsCount), MenuItemsCount);
            GLCD_print(64-15,7, Str);                                           // show navigation position in the menu
            GLCD_print(122-(strlen(VERSION)*6),7, (const far char *) "v"VERSION);// show software version in bottom right corner.
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

//
// Write 2 rows of data in GLCD buffer to LCD
//
void GLCD_sendbuf2(unsigned char RowAdr) {
    unsigned char i, x = 0, y = 0;

    do {
        goto_xy(0, RowAdr + y);
        for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                    //put data on data port
    } while (++y < 2);
}

//
// Write 4 rows of data in GLCD buffer to LCD
//
void GLCD_sendbuf4(unsigned char RowAdr) {
    unsigned int i, x = 0, y = 0;

    do {
        goto_xy(0, RowAdr + y);
        for (i = 0; i < 128; i++) st7565_data(GLCDbuf[x++]);                    //put data on data port
    } while (++y < 4);
}

void glcd_clear(void) {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        glcd_clrln(i, 0);
    }
}

void font_condense(unsigned char c, unsigned char *start, unsigned char *end, unsigned char space) {
    if(c >= '0' && c <='9') return;
    if(c == ' ' && space) return;
    if(font[(5 * c)] == 0) *start = 1;
    if(font[(5 * c) + 4] == 0) *end = 4;
}

void GLCD_write(unsigned int c) {
    unsigned char i=0, m=5;
    goto_xy(GLCDx, GLCDy);

    font_condense(c, &i , &m, 1);
    GLCDx = GLCDx + (m - i) + 1;

    do {
        st7565_data(font[(5 * c) + i]);
    } while (++i < m);
}

void GLCD_buffer_clr(void) {
    unsigned char x = 0;
    do {
        GLCDbuf[x++] = 0;                                                       // clear first 256 bytes of GLCD buffer
    } while (x != 0);
}

void GLCD_write_buf(unsigned int c) {
    unsigned int x;
    unsigned char i=0, m=5;

    x = 128 * GLCDy;
    x = x + GLCDx;

    font_condense(c, &i, &m, 1);                                                // remove whitespace from font
    GLCDx = GLCDx + (m - i) + 1;

    do {
        GLCDbuf[x++] = font[(5 * c) + i];
    } while (++i < m);
}

// Write one double height character to the GLCD buffer
// special characters '.' and ' ' will use reduced width in the buffer
void GLCD_write_buf2(unsigned int c) {
    unsigned char i=0, m=5, ch, z1;
    unsigned int x;
    x = GLCDx;

    font_condense(c, &i, &m, 0);
    GLCDx = GLCDx + ((m - i) * 2) +2;

    do {
        z1 = 0;
        ch = font[(5 * c) + i];
        if (ch & 0x01) z1 = z1 | 0x3;
        if (ch & 0x02) z1 = z1 | 0xc;
        if (ch & 0x04) z1 = z1 | 0x30;
        if (ch & 0x08) z1 = z1 | 0xc0;
        GLCDbuf[x] = z1;
        GLCDbuf[x + 1] = z1;
        z1 = 0;
        ch = ch >> 4;
        if (ch & 0x01) z1 = z1 | 0x3;
        if (ch & 0x02) z1 = z1 | 0xc;
        if (ch & 0x04) z1 = z1 | 0x30;
        if (ch & 0x08) z1 = z1 | 0xc0;
        GLCDbuf[x + 128] = z1;
        GLCDbuf[x + 129] = z1;
        x = x + 2;
    } while (++i < 5);
}

// Write a string directly to LCD
//
void GLCD_print(unsigned char x, unsigned char y, const far char* str) {
    unsigned char i = 0;

    GLCDx = x;
    GLCDy = y;
    while (str[i]) {
        GLCD_write(str[i++]);
    }
}

// Write a string to LCD buffer
void GLCD_print_buf(unsigned char x, unsigned char y, const far char* str) {
    unsigned char i = 0;

    GLCDx = x;
    GLCDy = y;
    while (str[i]) {
        GLCD_write_buf(str[i++]);
    }
}


void GLCD_init(void) {
    delay(200);                                                                 // transients on the line could have garbled the LCD, wait 200ms then re-init. (blocking)
    _A0_0;                                                                      // A0=0
    _RSTB_0;                                                                    // Reset GLCD module
    __delay_us(4);
    _RSTB_1;                                                                    // Reset line high
    __delay_us(4);

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
    glcd_clear();                                                               // Clear whole display
    GLCD_print_buf2(2, (const far char *) "Smart EVSE");
    GLCD_print_buf2(4, (const far char *) "Ver "VERSION);

    delay(2000);                                                                // show version for 2 seconds
}
