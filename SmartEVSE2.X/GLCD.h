/*
;	 Project:       Smart EVSE
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

#ifndef __GLCD_H
#define __GLCD_H

void GLCD_write(unsigned int c);
void GLCD_write2(unsigned int c);
void GLCD_write_buf(unsigned int c);
void GLCD_write_buf2(unsigned int c);
void GLCD_print(unsigned char x,unsigned char y,const char* str);
void GLCD_print_buf(unsigned char x,unsigned char y,const char* str);
void GLCD_print_buf2(unsigned char y,const char* str);
void GLCD_print_menu(unsigned char y,const char* str);
void glcd_clrln(unsigned char ln,unsigned char data);
void GLCD_sendbuf2(unsigned char RowAdr);
void GLCD_sendbuf4(unsigned char RowAdr);
void GLCD_buffer_clr(void);
void glcd_clear(void);
void font_condense(unsigned char c, unsigned char *start, unsigned char *end, unsigned char space);

extern void GLCDHelp(void);
extern void GLCD(void);
extern void GLCDMenu(unsigned char Buttons);
extern void GLCD_init(void);
extern void GLCD_version(void);

#endif // #ifndef __GLCD_H 