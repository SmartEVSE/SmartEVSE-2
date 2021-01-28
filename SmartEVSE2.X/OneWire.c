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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xc.h>

#include "EVSE.h"
#include "utils.h"
#include "OneWire.h"

unsigned char RFID[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char RFIDlist[120];                                                    // holds up to 20 RFIDs



// ############################# OneWire functions #############################



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



// ############################## RFID functions ##############################



// Read a list of 20 RFID's from eeprom
//
void ReadRFIDlist(void) {
    EEADR = 0;                                                                  // start from adr 256 in eeprom
    EEADRH = 1;

    eeprom_read_object(RFIDlist, 120);
}

// Write a list of 20 RFID's to the eeprom
//
void WriteRFIDlist(void) {
    char savint;

    unlock55 = unlockMagic + 0x33;
    unlockAA = unlockMagic + 0x88;                                              // set unlock variables to magic values

    savint = INTCON;                                                            // Save interrupts state
    INTCONbits.GIE = 0;                                                         // Disable interrupts

    EEADR = 0;                                                                  // start from adr 256 in eeprom
    EEADRH = 1;

    eeprom_write_object(RFIDlist, 120);                                         // write 120 bytes to eeprom

    unlock55 = 0;                                                               // clear unlock values
    unlockAA = 0;

    INTCON = savint;                                                            // Restore interrupts
#ifdef LOG_DEBUG_EVSE
    printf("\nRFID list saved\n");
#endif
}

// scan for matching RFID in RFIDlist
// returns offset+6 when found, 0 when not found
unsigned char MatchRFID(void) {
    unsigned char offset = 0, r;

    do {
        r = memcmp(RFID + 1, RFIDlist + offset, 6 );                            // compare read RFID with list of stored RFID's
        offset += 6;
    } while (r !=0 && offset < 114);

    if (r == 0) return offset;                                                  // return offset + 6 in RFIDlist
    else return 0;
}


// Store RFID card in memory and eeprom
// returns 1 when successful
// returns 2 when already stored
// returns 0 when all slots are full.
unsigned char StoreRFID(void) {
    unsigned char offset = 0, r;
    unsigned char empty[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

    // first check if the Card ID was already stored.
    if ( MatchRFID() ) return 2;                                                // already stored, that's ok.

    do {
        r = memcmp(empty, RFIDlist + offset, 6 );
        offset += 6;
    } while (r !=0 && offset < 120);
    if (r != 0) return 0;                                                       // no more room to store RFID
    offset -= 6;
//    printf("offset %u ",offset);
    memcpy(RFIDlist + offset, RFID+1, 6);

#ifdef LOG_DEBUG_EVSE
    printf("\nRFIDlist:");
    for (r=0; r<120; r++) printf("%02x",RFIDlist[r]);
#endif

    WriteRFIDlist();
    return 1;
}

// Delete RFID card in memory and eeprom
// returns 1 when successful, 0 when RFID was not found
unsigned char DeleteRFID(void) {
    unsigned char offset = 0, r;

    offset = MatchRFID();                                                       // find RFID in list
    if (offset) {
        offset -= 6;
        for (r = 0; r < 6; r++) RFIDlist[offset + r] = 0xff;
    } else return 0;

//    printf("deleted %u ",offset);
//    for (r=0; r<120; r++) printf("%02x",RFIDlist[r]);

    return 1;
}

void DeleteAllRFID(void) {
    unsigned char i;

    for (i = 0; i < 120; i++) RFIDlist[i] = 0xff;
    WriteRFIDlist();
#ifdef LOG_INFO_EVSE
    printf("\nAll RFID cards erased!");
#endif
    RFIDReader = 0;                                                             // RFID Reader Disabled
}

void CheckRFID(void) {
    unsigned char x;

    // When RFID is enabled, a OneWire RFID reader is expected on the SW input
    if (RFIDReader) {                                                           // RFID Reader set to Enabled, Learn or Delete
        if (OneWireReadCardId() ) {                                             // Read card ID
            switch (RFIDReader) {
                case 1:                                                         // Enabled
                    x = MatchRFID();
                    if (x && !RFIDstatus) {
                        //printf("RFID card found!\n");
                        if (Access_bit) {
                            Access_bit = 0;                                     // Toggle Access bit on/off
                            setState(STATE_A);                                  // Switch back to state A
                        } else Access_bit = 1;

                        RFIDstatus = 1;
                    }  else if (!x) RFIDstatus = 7;                             // invalid card
                    BacklightTimer = BACKLIGHT;
                    break;
                case 2:                                                         // Learn Card
                    x = StoreRFID();
                    if (x == 1) {
                        printf("\nRFID card stored!");
                        RFIDstatus = 2;
                    } else if (x == 2 && !RFIDstatus) {
                        printf("\nRFID card was already stored!");
                        RFIDstatus = 4;
                    } else if (!RFIDstatus) {
                        printf("\nRFID storage full! Delete card first");
                        RFIDstatus = 6;
                    }
                    break;
                case 3:                                                         // Delete Card
                    x = DeleteRFID();
                    if (x) {
                        printf("\nRFID card deleted!");
                        RFIDstatus = 3;
                    } else if (!RFIDstatus) {
                        printf("\nRFID card not in list!");
                        RFIDstatus = 5;
                    }
                    break;
                default:
                    break;
            }
        } else RFIDstatus = 0;
    }
}