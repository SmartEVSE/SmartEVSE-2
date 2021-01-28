/*
 * File:   utils.c
 * Author: Mike
 *
 * Created on 11 januari 2020, 21:44
 */

#include <stdio.h>
#include <stdlib.h>
#include "EVSE.h"
#include "utils.h"


// Reset 1-Wire device on IO2 (SW input)
// returns:  1 Device found
//           0 No device found
//         255 Error. Line is pulled low (short, or external button pressed?)
//
unsigned char OneWireReset(void) {
    unsigned char r, savint;

    if (PORTBbits.RB2 == 0) return 255;                                         // Error, pulled low by external device?

    ONEWIRE_LOW;                                                                // Drive wire low
    __delay_us(480);
    savint = INTCON;                                                            // Save interrupts state
    INTCONbits.GIE = 0;                                                         // Disable interrupts
	ONEWIRE_FLOATHIGH;                                                          // don't drive high, but use pullup
    __delay_us(70);
    if (PORTBbits.RB2 == 1) r = 0;                                              // sample pin to see if there is a OneWire device..
    else r = 1;
    INTCON = savint;                                                            // Restore interrupts
	__delay_us(410);
    return r;
}

void OneWireWriteBit(unsigned char v) {
    unsigned char savint;

    if (v & 1) {                                                                // write a '1'
        savint = INTCON;                                                        // Save interrupts state
        INTCONbits.GIE = 0;                                                     // Disable interrupts
        ONEWIRE_LOW;                                                            // Drive low
        __delay_us(10);
        ONEWIRE_HIGH;                                                           // Drive high
        INTCON = savint;                                                        // Restore interrupts
        __delay_us(55);
    } else {                                                                    // write a '0'
        savint = INTCON;                                                        // Save interrupts state
        INTCONbits.GIE = 0;                                                     // Disable interrupts
        ONEWIRE_LOW;                                                            // Drive low
        __delay_us(65);
        ONEWIRE_HIGH;                                                           // Drive high
        INTCON = savint;                                                        // Restore interrupts
        __delay_us(5);
    }
}

unsigned char OneWireReadBit(void) {
    unsigned char r, savint;

    savint = INTCON;                                                            // Save interrupts state
    INTCONbits.GIE = 0;                                                         // Disable interrupts
    ONEWIRE_LOW;
    __delay_us(3);
    ONEWIRE_FLOATHIGH;
    __delay_us(10);
    if (PORTBbits.RB2) r = 1;                                                   // sample pin
    else r = 0;
    INTCON = savint;                                                            // Restore interrupts
    __delay_us(53);
    return r;
}

void OneWireWrite(unsigned char v) {
    unsigned char bitmask;
    for (bitmask = 0x01; bitmask ; bitmask <<= 1) {
        OneWireWriteBit( (bitmask & v)?1:0);
    }
}

unsigned char OneWireRead(void) {
    unsigned char bitmask, r = 0;

    for (bitmask = 0x01; bitmask ; bitmask <<= 1) {
        if ( OneWireReadBit()) r |= bitmask;
    }
    return r;
}

unsigned char crc8(unsigned char *buf, unsigned char len) {
	unsigned char crc = 0, i, mix, inbyte;

	while (len--) {
		inbyte = *buf++;
		for (i = 8; i; i--) {
			mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}

/**
 * Calculates 16-bit CRC of given data
 * used for Frame Check Sequence on data frame
 * 
 * @param unsigned char pointer to buffer
 * @param unsigned char length of buffer
 * @return unsigned int CRC
 */
unsigned int crc16(unsigned char *buf, unsigned char len) {
    unsigned int pos, crc = 0xffff;
    unsigned char i;

    // Poly used is x^16+x^15+x^2+x
    for (pos = 0; pos < len; pos++) {
        crc ^= (unsigned int)buf[pos];                                          // XOR byte into least sig. byte of crc

        for (i = 8; i != 0; i--) {                                              // Loop over each bit
            if ((crc & 0x0001) != 0) {                                          // If the LSB is set
                crc >>= 1;                                                      // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else                                                              // Else LSB is not set
                crc >>= 1;                                                      // Just shift right
        }
    }

    return crc;
}

/**
 * Delay for number of mS (blocking)
 * 
 * @param unsigned int d: Number or milliseconds
 */
void delay(unsigned int d) {
    unsigned long x;
    x = Timer;                                                                  // read Timer value (increased every ms)
    while (Timer < (x + d));
}

unsigned char OneWireReadCardId(void) {
    unsigned char x;

    if (OneWireReset() == 1) {                                                  // RFID card detected
        OneWireWrite(0x33);                                                     // OneWire ReadRom Command
        for (x=0 ; x<8 ; x++) RFID[x] = OneWireRead();                          // read Family code (0x01) RFID ID (6 bytes) and crc8
        if (crc8(RFID,8)) {
            RFID[0] = 0;                                                        // CRC incorrect, clear first byte of RFID buffer
            return 0;
        } else {
//            for (x=1 ; x<7 ; x++) printf("%02x",RFID[x]);
//            printf("\r\n");
            return 1;
        }
    }
    return 0;
}

/**
 * Insert rounded value into string in printf style
 * 
 * @param pointer to string
 * @param string Format
 * @param signed long Value to round and insert
 * @param unsinged char Divisor where to set decimal point
 * @param unsigned char Decimal place count
 */
void sprintfl(unsigned char *str, const unsigned char *Format, signed long Value, unsigned char Divisor, unsigned char Decimal) {
    signed long val;

    val = Value / (signed long) pow10[Divisor - Decimal - 1];
    // Round value
    if(val < 0) val -= 5;
    else val += 5;
    val /= 10;
    // Split value
    if(Decimal > 0) sprintf(str, Format, (signed int) (val / (signed long) pow10[Decimal]), (unsigned int) (abs(val) % pow10[Decimal]));
    else sprintf(str, Format, (signed int) val);
}

/* triwave8: triangle (sawtooth) wave generator.  Useful for
           turning a one-byte ever-increasing value into a
           one-byte value that oscillates up and down.

           input         output
           0..127        0..254 (positive slope)
           128..255      254..0 (negative slope)
 */
unsigned char triwave8(unsigned char in) {
    if (in & 0x80) {
        in = 255u - in;
    }
    unsigned char out = in << 1;
    return out;
}

unsigned char scale8(unsigned char i, unsigned char scale) {
    return (((unsigned int) i) * (1 + (unsigned int) (scale))) >> 8;
}

/* easing functions; see http://easings.net

    ease8InOutQuad: 8-bit quadratic ease-in / ease-out function
 */
unsigned char ease8InOutQuad(unsigned char i) {
    unsigned char j = i;
    if (j & 0x80) {
        j = 255u - j;
    }
    unsigned char jj = scale8(j, j);
    unsigned char jj2 = jj << 1;
    if (i & 0x80) {
        jj2 = 255u - jj2;
    }
    return jj2;
}
