/*
;	 Project:       Smart EVSE
;    Date:          1 February 2016
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
	0x10, 0x10, 0x10, 0xFF, 0x10,
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
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0x7C, 0x2A, 0x2A, 0x3E, 0x14,
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
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
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00
}; 

const far char StrConfig[] = 	"CONFIG";
const far char StrMode[] = 		"MODE";
const far char StrFixed[] = 	"Fixed";
const far char StrSocket[] = 	"Socket";
const far char StrSmart[] =		"Smart";
const far char StrNormal[] =	"Normal";
const far char StrMains[] = 	"MAINS";
const far char StrMax[] =	 	"MAX";
const far char StrMin[] = 		"MIN";
const far char StrLock[] = 		"LOCK";
const far char StrSolenoid[] =	"Solenoid";
const far char StrMotor[] =		"Motor";
const far char StrDisable[] =	"Disabled";
const far char StrCable[] = 	"CABLE";
const far char StrCal[] =	 	"CAL";
const far char StrLoadBl[] =	"LOAD BAL";
const far char StrMaster[] =	"Master";
const far char StrSlave1[] =	"Slave 1";
const far char StrSlave2[] =	"Slave 2";
const far char StrSlave3[] =	"Slave 3";
const far char StrAccess[] = 	"ACCESS";
const far char StrSwitch[] =	"Switch";
const far char StrExit[] =	 	"EXIT";
const far char StrExitMenu[] =	"MENU";


unsigned int GLCDx,GLCDy;

// uses buffer
void GLCD_print_row(const far char *data )	// write 10 characters to LCD
{
	unsigned char x=0;
	GLCDx=4;

	GLCD_buffer_clr();										// Clear buffer
	do
  	{
    	GLCD_write_buf2(*data++);
  	} while (x++<9);
	GLCD_sendbuf(2);									// copy buffer to LCD
}

void GLCD_print_arrows(void)
{
		GLCDx=0;
		GLCD_write_buf2('<');
		GLCDx=9*12+8;										// last character of line
		GLCD_write_buf2('>');
}


// uses buffer
void GLCD_print_menu(const far char *data,char RowAdr )	// write string of data to LCD, with navigation arrows
{
	GLCD_buffer_clr();										// Clear buffer

	if ((SubMenu && RowAdr==4) || (!SubMenu && RowAdr==2))	// navigation arrows
	{
		GLCD_print_arrows();
	}

	GLCDx=64-(strlen(data)*6);							// calculate offset for centering text
	do
  	{
    	GLCD_write_buf2(*data);
  	} while (*++data);

	GLCD_sendbuf(RowAdr);									// copy buffer to LCD
}


// uses buffer
void GLCD_print_Amps(unsigned int Amps )					// write data to LCD
{
	GLCD_buffer_clr();										// Clear buffer

	if (SubMenu)
	{
		GLCD_print_arrows();
	}

	if (Amps>=100) 
	{
		GLCDx=40;
		GLCD_write_buf2( (Amps/100)+0x30 );
		Amps=Amps%100;
	}
	else GLCDx=46;
	GLCD_write_buf2( (Amps/10)+0x30 );
	GLCD_write_buf2( (Amps%10)+0x30 );
	GLCD_write_buf2('A');

	GLCD_sendbuf(4);									// copy buffer to LCD
}

// uses buffer
void GLCDHelp(void)							// Display/Scroll helptext on LCD 
{
		unsigned int x;

			switch (LCDNav)
			{
				case MENU_CONFIG:
						x=strlen(MenuConfig);
						GLCD_print_row(MenuConfig+LCDpos);
						break;
				case MENU_MODE:
						x=strlen(MenuMode);
						GLCD_print_row(MenuMode+LCDpos);
						break;
                case MENU_LOADBL:
						x=strlen(MenuLoadBl);
						GLCD_print_row(MenuLoadBl+LCDpos);
						break;
				case MENU_MAINS:
						x=strlen(MenuMains);
						GLCD_print_row(MenuMains+LCDpos);
						break;
				case MENU_MAX:
						x=strlen(MenuMax);
						GLCD_print_row(MenuMax+LCDpos);
						break;
				case MENU_MIN:
						x=strlen(MenuMin);
						GLCD_print_row(MenuMin+LCDpos);
						break;
				case MENU_LOCK:
						x=strlen(MenuLock);
						GLCD_print_row(MenuLock+LCDpos);
						break;
				case MENU_CABLE:
						x=strlen(MenuCable);
						GLCD_print_row(MenuCable+LCDpos);
						break;
				case MENU_CAL:
						x=strlen(MenuCal);
						GLCD_print_row(MenuCal+LCDpos);
						break;
                case MENU_ACCESS:
						x=strlen(MenuAccess);
						GLCD_print_row(MenuAccess+LCDpos);
						break;
                        
				default:
						break;
			}
			if (LCDpos++==8) ScrollTimer=Timer-4000;
			else if (LCDpos > (x-10) )
			{
				ScrollTimer=Timer-3000;
				LCDpos=8;
			}
			else ScrollTimer=Timer-4700;
}




// called once a second
void GLCD(void)
{
		unsigned char x;

		if (LCDNav)
		{
			if (LCDTimer++ == 120)
			{	 
				LCDNav=0;						// Exit Setup menu after 120 seconds.
				read_settings();				// don't save, but restore settings
			}
			else return;						// disable LCD status messages when navigating LCD Menu	
		}

		if (LCDTimer==10) LCDTimer=0;

		if (Error)
		{
			BACKLIGHT_ON; 						// LCD backlight on

			if (Error==LESS_6A)
			{
				GLCD_print2(2,(const far char *)"ERROR NO");
				GLCD_print2(4,(const far char *)"CURRENT");
			}
			else if (Error==CT_NOCOMM)
			{
				GLCD_print2(2,(const far char *)"ERROR NO");
				GLCD_print2(4,(const far char *)"SERIAL COM");
			}
			else if (Error==TEMP_HIGH)
			{
				GLCD_print2(2,(const far char *)"ERROR");
				GLCD_print2(4,(const far char *)"HIGH TEMP");
			}
			else if (Error==Test_IO)								// Only used when testing the module
			{
				GLCD_print2(2,(const far char *)"IO Test");
				GLCD_print2(4,(const far char *)"FAILED!   ");
				GLCDx=12*8+4;
				GLCD_write_buf2((TestState/10)+0x30 );
				GLCD_write_buf2((TestState%10)+0x30 );
				GLCD_sendbuf(4);									// copy buffer to LCD
			}

			return;
		}
		
		if (TestState==80)											// Only used when testing the module
		{
			GLCD_print2(2,(const far char *)"IO Test");
			GLCD_print2(4,(const far char *)"Passed");
			return;
		}


		if ((LCDTimer++>4) && Mode)			
		{
			GLCD_print2(2,(const far char *)"L1 L2 L3");
			
			GLCD_buffer_clr();										// Clear buffer
			for (x=0; x<3 ;x++)
			{
				GLCDx=4+12+x*(12*3);								// calc offset
				GLCD_write_buf2((unsigned int)(Irms[x]/100)+0x30 );
				GLCD_write_buf2(((unsigned int)(Irms[x]/10)%10)+0x30 );
			}
			GLCD_sendbuf(4);                                        // copy buffer to LCD
		}
		else if ((State == STATE_A) || (State == STATE_B))
		{
			BACKLIGHT_OFF;                                          // LCD backlight off
            glcd_clrln(0,0x00);
        	glcd_clrln(1,0x04);                                     // horizontal line
			GLCD_print2(2,(const far char *)"READY TO");
			GLCD_print2(4,(const far char *)"CHARGE  ");
         	glcd_clrln(6,0x10);                                     // horizontal line
            glcd_clrln(7,0x00);
            
			if (ChargeDelay>0)
			{
				GLCDx=12*8+4;
				GLCD_write_buf2((ChargeDelay/10)+0x30 );
				GLCD_write_buf2((ChargeDelay%10)+0x30 );
				GLCD_sendbuf(4);									// copy buffer to LCD
			}
			
		}
		else if (State == STATE_C)
		{
			BACKLIGHT_ON;					// LCD backlight on

			GLCD_print2(2,(const far char *)"CHARGING");

			GLCDx=4+12;
			GLCD_buffer_clr();										// Clear buffer
			GLCD_write_buf2((Balanced[0]/10)+0x30 );
			GLCD_write_buf2((Balanced[0]%10)+0x30 );
			GLCD_write_buf2('A');
			if (Mode)						// Smart Mode?
			{
				GLCD_write_buf2('(');				
				GLCD_write_buf2((MaxMains/10)+0x30 );
				GLCD_write_buf2((MaxMains%10)+0x30 );
				GLCD_write_buf2('A');
				GLCD_write_buf2(')');
			} 
			GLCD_sendbuf(4);									// copy buffer to LCD
		}

}


//##############################################################################################################################
// 10 CONFIG			- Set to Fixed Cable or Type 2 Socket
// 20 MODE  			- Set to Smart mode, or Normal EVSE mode
// 30		MAINS 		- Set max MAINS Current (25-100) (Mode=Smart)
// 40 MAX   			- Set MAX Charge Current for the EV (16-80)
// 50		MIN   		- Set MIN Charge Current the EV will accept (Mode=Smart)
// 60 LOCK				- Cable lock Enable/disable
// 70 CABLE				- Set Fixed Cable Current limit 
// 80 CAL 		  		- Calibrate CT1
// 90 EXIT				- Exit Menu
//100 LOADBL            - Load Balancing
//110 ACCESS            - Access control on IO2


void GLCDMenu(unsigned char Buttons)
{
	static unsigned long ButtonTimer=0;
	static unsigned char ButtonRelease=0;			// keeps track of LCD Menu Navigation
	static unsigned int CT1,CT1old;
	static double Iold;
	
// Main Menu Navigation

	if ((LCDNav==0) && (Buttons==0x5) && (ButtonRelease==0))		// Button 2 pressed ?
	{
		LCDNav=1;							// about to enter menu
		ButtonTimer=Timer;					
	}
	else if  (LCDNav==1 && ((ButtonTimer+2000)<Timer))		// <CONFIG>
	{
		LCDNav=MENU_CONFIG;					// Main Menu entered
		BACKLIGHT_ON;						// LCD backlight on
		ButtonRelease=1;
	}
	else if ((LCDNav==1) && (Buttons==0x7))	// Button 2 released before entering menu?
	{
		LCDNav=0;
		ButtonRelease=0;
		GLCD();
	}
	else if ( (LCDNav>0) && ((LCDNav%10)==0) && (Buttons==0x3) && (ButtonRelease==0) )		// Button 1 > pressed 
	{
		switch (LCDNav)
		{
			case MENU_CONFIG:
					if (SubMenu)
					{
						if (Config) Config=0;
						else Config=1;
					} 
					else LCDNav=MENU_MODE;
					break;
			case MENU_MODE:
					if (SubMenu)
					{
						if (Mode) Mode=0;
						else Mode=1;
					} 
                    else LCDNav=MENU_LOADBL;
					break;
			case MENU_LOADBL:
					if (SubMenu)
					{
						if (LoadBl==4) LoadBl=0;					// last menu item? goto first
						else LoadBl++;								// goto next
					} 
					else 
					{
						if (Mode || (LoadBl==1)) LCDNav=MENU_MAINS;	// Smart Mode or Master?
						else LCDNav=MENU_MAX;
					}
					break;
			case MENU_MAINS:
					if (SubMenu)
					{
						MaxMains++;									// Set new MaxMains
						if (MaxMains>100) MaxMains=100;				// Max 100A
					}
					else LCDNav=MENU_MAX;
					break;
			case MENU_MAX:
					if (SubMenu)
					{
						MaxCurrent++;								// Set new MaxCurrent
						if (MaxCurrent>80) MaxCurrent=80;			// Max 80A
					}
					else
					{
						if (Mode || (LoadBl==1)) LCDNav=MENU_MIN;	// Smart Mode or Master?
						else if (Config) LCDNav=MENU_CABLE;			// Cable Configuration, go to Cable Current
						else LCDNav=MENU_LOCK;						// Fixed Cable, use the lock
					}
					break;
			case MENU_MIN:
					if (SubMenu)
					{
						MinCurrent++;								// Set new MinCurrent
						if (MinCurrent>16) MinCurrent=16;			// Max 16A
					}
					else
					{
						if (Config) LCDNav=MENU_CABLE;				// Cable Configuration, go to Cable Current
						else LCDNav=MENU_LOCK;						// Fixed Cable, use the lock
					}
					break;
			case MENU_LOCK:
					if (SubMenu)
					{
						if (Lock==2) Lock=0;
						else Lock++;
						break;
					} 
			case MENU_CABLE:
					if (SubMenu)
					{
						CableLimit++;								// Set new CableLimit
						if (CableLimit>80) CableLimit=80;			// Max 80A
					}
					else 
					{
						if (Mode) LCDNav=MENU_CAL;
						else LCDNav=MENU_ACCESS;
					}
					break;
			case MENU_CAL:
					if (SubMenu)
					{
						if (CT1>=60 && CT1<1000) CT1++; 			// Increase CT1 measurement value by 0.1A
																	// Max 99.9A
					}
					else 
					{
						LCDNav=MENU_ACCESS;
					}
					break;
            case MENU_ACCESS:
					if (SubMenu)
					{
						if (Access) Access=0;
						else Access=1;
					}
                    else 
                    {
                        LCDNav=MENU_EXIT;
                    }
                    break;
			case MENU_EXIT:
					LCDNav=MENU_CONFIG;
			default:
					break;
		}
		ButtonRelease=1;
	}
	else if ( (LCDNav>0) && ((LCDNav%10)==0) && (Buttons==0x6) && (ButtonRelease==0) )		// Button 3 < pressed 
	{
		switch (LCDNav)
		{
			case MENU_EXIT:
                    LCDNav=MENU_ACCESS;
					break;
            case MENU_ACCESS:
                    if (SubMenu)
                    {
                        if (Access) Access=0;
                        else Access=1;
                    }
                    else if (Mode) LCDNav=MENU_CAL;				// Smart Mode? Goto Cal CT1
					else if (Config) LCDNav=MENU_CABLE;			// Cable Configuration, go to Cable Current
					else LCDNav=MENU_LOCK;						// Fixed Cable, use the lock
                    break;
			case MENU_CAL:
					if (SubMenu)
					{
						if (CT1>60) CT1--;						// Min 6.0A
					}
					else 
					{
						if (Config) LCDNav=MENU_CABLE;			// Cable Configuration, go to Cable Current
						else LCDNav=MENU_LOCK;					// Fixed Cable, use the lock
					}
					break;
			case MENU_CABLE:
					if (SubMenu)
					{
						CableLimit--;							// Set new CableLimit
						if (CableLimit<13) CableLimit=13;		// Min 13A
						break;
					}
			case MENU_LOCK:
					if (SubMenu)
					{
						if (Lock==0) Lock=2;
						else Lock--;
					} 
					else
					{
						if (Mode || (LoadBl==1)) LCDNav=MENU_MIN;	// Smart Mode or Master?
						else LCDNav=MENU_MAX;
					}
					break;
			case MENU_MIN:
					if (SubMenu)
					{
						MinCurrent--;							// Set new MinCurrent
						if (MinCurrent<6) MinCurrent=6;			// Min 6A
					}
					else LCDNav=MENU_MAX;
					break;
			case MENU_MAX:
					if (SubMenu)
					{
						MaxCurrent--;							// Set new MaxCurrent
						if (MaxCurrent<10) MaxCurrent=10;		// Min 10A
					}
					else
					{
						if (Mode || (LoadBl==1)) LCDNav=MENU_MAINS;	// Smart Mode or Master?
						else LCDNav=MENU_LOADBL;
					}
					break;
			case MENU_MAINS:
					if (SubMenu)
					{
						MaxMains--;								// Set new MaxMains
						if (MaxMains<10) MaxMains=10;			// Min 10A (version 2.03 changed from 16A)
					} else LCDNav=MENU_LOADBL;                  
					break;
			case MENU_LOADBL:
					if (SubMenu)
					{
						if (LoadBl==0) LoadBl=4;					// first menu item? goto last
						else LoadBl--;								// goto previous
					} 
					else LCDNav=MENU_MODE;
					break;
			case MENU_MODE:
					if (SubMenu)
					{
						if (Mode) Mode=0;
						else Mode=1;
					} 
					else LCDNav=MENU_CONFIG;
					break;
			case MENU_CONFIG:
					if (SubMenu)
					{
						if (Config) Config=0;
						else Config=1;
					} 
					else LCDNav=MENU_EXIT;
                    break;
			
            default:
					break;
		}
		ButtonRelease=1;
	}
	else if (LCDNav>=10 && Buttons==0x5 && ButtonRelease==0)		// Button 2 pressed?
	{
		if (SubMenu)									// Are we in Submenu?
		{
			SubMenu=0;									// yes, exit Submenu
			if (LCDNav==MENU_CAL)						// Exit CT1 calibration?
			{
				if (CT1!=CT1old)						// did the value change?
				{
					Iold=(double)(CT1old/ICal);
					ICal=(double)(CT1/Iold);			// Calculate new Calibration value
					Irms[0]=CT1;						// Set the Irms value, so the LCD update is instant
				}
			}
		}
		else 											// We are curently not in Submenu.
		{
			SubMenu=1;									// Enter Submenu now
			if (LCDNav==MENU_CAL)						// CT1 calibration start
			{
				CT1=(unsigned int)Irms[0];				// make working copy of CT1 value
				CT1old=CT1;								// and a backup
			}
			else if (LCDNav==MENU_EXIT)					// Exit Main Menu
			{
				LCDNav=0;			
				SubMenu=0;
				BACKLIGHT_OFF;  						// LCD backlight off
				Error=NO_ERROR;							// Clear Errors
				TestState=0;							// Clear TestState
				write_settings();						// Write to eeprom
				GLCD();
			}
		}
		ButtonRelease=1;
	}	
	else if (Buttons==0x7)		// Buttons released
	{
		ButtonRelease=0;
		delay(10);              // debounce keys
	}	

//	printf((const far char *)"But:%02x release %02x \r\n",Buttons,ButtonRelease);

//
// here we update the LCD
//
	if (ButtonRelease==1 || LCDNav==1)
	{
		if (LCDNav==1)
		{
			GLCD_print2(2,(const far char *)"Hold 2 sec");
			GLCD_print2(4,(const far char *)"for Menu");
		}
		else if (LCDNav==MENU_CONFIG)
		{
			GLCD_print_menu(StrConfig,2);			// add navigation arrows on both sides
			if (Config)	GLCD_print_menu(StrFixed,4);	// add spaces on both sides
			else GLCD_print_menu(StrSocket,4);
		}
		else if (LCDNav==MENU_MODE)
		{ 
			GLCD_print_menu(StrMode,2);
			if (Mode) GLCD_print_menu(StrSmart,4);
			else GLCD_print_menu(StrNormal,4);
		}
        else if (LCDNav==MENU_LOADBL)
        { 
            GLCD_print_menu(StrLoadBl,2);
            if (LoadBl==0) GLCD_print_menu(StrDisable,4);
            else if (LoadBl==1) GLCD_print_menu(StrMaster,4);
            else if (LoadBl==2) GLCD_print_menu(StrSlave1,4);
            else if (LoadBl==3) GLCD_print_menu(StrSlave2,4);
            else GLCD_print_menu(StrSlave3,4);
        }
		else if (LCDNav==MENU_MAINS)
		{ 
			GLCD_print_menu(StrMains,2);
			GLCD_print_Amps(MaxMains);
		}
		else if (LCDNav==MENU_MAX)
		{ 
			GLCD_print_menu(StrMax,2);
			GLCD_print_Amps(MaxCurrent);
		}
		else if (LCDNav==MENU_MIN)
		{ 
			GLCD_print_menu(StrMin,2);
			GLCD_print_Amps(MinCurrent);
		}
		else if (LCDNav==MENU_LOCK)
		{
			GLCD_print_menu(StrLock,2);
			if (Lock==1) GLCD_print_menu(StrSolenoid,4);
            else if (Lock==2) GLCD_print_menu(StrMotor,4);
			else GLCD_print_menu(StrDisable,4);
		}
		else if (LCDNav==MENU_CABLE)
		{ 
			GLCD_print_menu(StrCable,2);
			GLCD_print_Amps(CableLimit);
		}
		else if (LCDNav==MENU_CAL)
		{ 
			GLCD_print_menu(StrCal,2);

			GLCD_buffer_clr();										// Clear buffer
			if (SubMenu) 
			{
				GLCD_print_arrows();
				GLCDx=4+(12*3);
				GLCD_write_buf2((CT1/100)+0x30 );
				GLCD_write_buf2((CT1%100)/10+0x30 );
				GLCD_write_buf2('.');
				GLCD_write_buf2((CT1%10)+0x30 );
			}
			else 
			{
				GLCDx=4+(12*3);
				GLCD_write_buf2(((unsigned int)Irms[0]/100)+0x30 );
				GLCD_write_buf2(((unsigned int)Irms[0]%100/10)+0x30 );
				GLCD_write_buf2('.');
				GLCD_write_buf2(((unsigned int)Irms[0]%10)+0x30 );
			}
			GLCDx=4+(12*7);
			GLCD_write_buf2('A');
			GLCD_sendbuf(4);									// copy buffer to LCD

		}
        else if (LCDNav==MENU_ACCESS)
		{ 
			GLCD_print_menu(StrAccess,2);
			if (Access) GLCD_print_menu(StrSwitch,4);
            else GLCD_print_menu(StrDisable,4);
			
		}
		else if (LCDNav==MENU_EXIT)
		{
			GLCD_print_menu(StrExit,2);
			GLCD_print_menu(StrExitMenu,4);
		}
		ButtonRelease=2;					// Set value to 2, so that LCD will be updated only once
	}	

	ScrollTimer=Timer;						// reset timer for HelpMenu text
	LCDpos=8;								// reset position of scrolling text
	OldButtonState=Buttons;
	LCDTimer=0;

}



void st7565_command(unsigned char data)
{
	_A0_0;	
	PIR1bits.SSP1IF=0;				// clear flag
	SSP1BUF=data;					// and send SPI data
	while(!PIR1bits.SSP1IF);		// wait for bit to become set
}
void st7565_data(unsigned char data)
{
	_A0_1;	
	PIR1bits.SSP1IF=0;				// clear flag
	SSP1BUF=data;					// and send SPI data
	while(!PIR1bits.SSP1IF);		// wait for bit to become set
}

void goto_row(unsigned char y)
{    
	unsigned char pattern;   
	pattern=0xB0 | (y & 0xBF); //put row address on data port set command     
	st7565_command(pattern);                
}
//--------------------
void goto_col(unsigned char x)
{
	unsigned char pattern;
	pattern=((0xF0&x)>>4)|0x10;
	st7565_command(pattern);	//set high byte column command
	pattern=((0x0F&x))|0x00;
	st7565_command(pattern);	//set low byte column command;
}
//--------------------
void goto_xy(unsigned char x,unsigned char y)
{    
    goto_col(x);
    goto_row(y);  
}

void glcd_clrln(unsigned char ln,unsigned char data)
{
	unsigned char i;
	goto_xy(0,ln);
	for(i=0;i<132;i++)
	{
		st7565_data(data);    //put data on data port  
	}
}

void GLCD_sendbuf(unsigned char RowAdr)
{
	unsigned char i,x=0;

	goto_xy(0,RowAdr);
	for(i=0;i<128;i++) st7565_data(GLCDbuf[x++]);    //put data on data port  

	goto_xy(0,RowAdr+1);
	for(i=0;i<128;i++) st7565_data(GLCDbuf[x++]);    //put data on data port  
}

void glcd_clear(void)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		glcd_clrln(i,0);
	}
}

void GLCD_write(unsigned int c)
{
	unsigned char i;
	goto_xy(GLCDx,GLCDy);
	for(i=0;i<5;i++)
	{
		st7565_data(font[(5*c)+i]);
	}
	GLCDx=GLCDx+6;
}

void GLCD_buffer_clr(void)
{	
	unsigned char x=0;
	do 
	{
		GLCDbuf[x++]=0;						// clear GLCD buffer
	} while (x!=0);
}


void GLCD_write_buf2(unsigned int c)
{
	unsigned char i,ch,z1,x;
	x=GLCDx;
	for(i=0;i<5;i++)
	{
		z1=0;
		ch=font[(5*c)+i];
		if (ch&0x01) z1=z1|0x3;
		if (ch&0x02) z1=z1|0xc;
		if (ch&0x04) z1=z1|0x30;
		if (ch&0x08) z1=z1|0xc0;
		GLCDbuf[x++]=z1;		
		GLCDbuf[x++]=z1;		
	}
	x=GLCDx+128;
	for(i=0;i<5;i++)
	{
		z1=0;
		ch=font[(5*c)+i]>>4;
		if (ch&0x01) z1=z1|0x3;
		if (ch&0x02) z1=z1|0xc;
		if (ch&0x04) z1=z1|0x30;
		if (ch&0x08) z1=z1|0xc0;
		GLCDbuf[x++]=z1;		
		GLCDbuf[x++]=z1;		
	}
	GLCDx=GLCDx+12;
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

void GLCD_print(unsigned char x,unsigned char y,const far char* str)
{
	unsigned int i=0;
	unsigned int ascii;

	GLCDx=x;
	GLCDy=y;
    for(i=0;i<128;i++)
	{   
		if(str[i]==0)
			break;
		ascii=str[i];
		GLCD_write(ascii);
	}
}


void GLCD_print2(unsigned char y,const far char* data)
{
	GLCD_buffer_clr();										// Clear buffer

	GLCDx=64-(strlen(data)*6);                              // calculate offset for centering text
	do
  	{
    	GLCD_write_buf2(*data);
  	} while (*++data);

	GLCD_sendbuf(y);                                        // copy buffer to LCD
}

void delayus(int us)
{
    while(us--) {};
}

void GLCD_init(void)
{
	_A0_0;							// A0=0
	_RSTB_0;						// Reset GLCD module
	delayus(4);
	_RSTB_1;						// Reset line high
	delayus(4);

    st7565_command(0xA2);			// set bias at duty cycle 1.65 (0xA2=1.9 0xA3=1.6)
	st7565_command(0xC8);			// comm direction normal =0xC0 comm reverse= 0xC8
	st7565_command(0xA0);			// seg dir (0xA0 or 0xA1)
	st7565_command(0xA6);			// set inverse (0xA7=inverse 0xA6=normal)

	st7565_command(0x20 |0x04);		// set Regulation Ratio (0-7)

	st7565_command(0xF8);			// send Booster command
	st7565_command(0x01);			// set Booster value 00=4x 01=5x

	st7565_command(0x81);			// send Electronic Volume command 0x81
	st7565_command(0x24);			// set Electronic volume (0x00-0x3f)

	st7565_command(0x28 |0x07);		// ALL Power Control ON
	st7565_command(0x40);			// Set display start line

	goto_row(0x00);					// Set page address
 	goto_col(0x00);               	// Set coloumn addr LSB
	st7565_command(0xAF); 			// ON command  
    
}


void GLCD_version(void)
{
	GLCD_print2(2,(const far char *)"Smart EVSE");
	GLCD_print2(4,(const far char *)"Ver "VERSION);
	
    delay(2000);        // show version for 2 seconds
}

