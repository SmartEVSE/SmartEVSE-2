/*
;    Project:       Smart EVSE
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

#ifndef __EVSE_MAIN
#define __EVSE_MAIN

#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "GLCD.h"


#define VERSION "2.07"                                                          // SmartEVSE software version
#define DEBUG_P                                                                 // Debug print enable/disable

#define ICAL 3.00                                                               // Irms Calibration value (for Current transformers) 
#define MAX_MAINS 25                                                            // max Current the Mains connection can supply
#define MAX_CURRENT 13                                                          // max charging Current for the EV
#define MIN_CURRENT 6                                                           // minimum Current the EV will accept
#define MODE 0                                                                  // Normal EVSE mode
#define LOCK 0                                                                  // No Cable lock
#define CABLE_LIMIT 13                                                          // Manual set Cable Limit (for use with Fixed Cable)
#define CONFIG 0                                                                // Configuration: 0= TYPE 2 socket, 1= Fixed Cable
#define LOADBL 0                                                                // Load Balancing disabled
#define ACCESS 0                                                                // 0= Charge on plugin, 1= (Push)Button on IO2 is used to Start/Stop charging.
#define RC_MON 0                                                                // Residual Current Monitoring on IO3. Disabled=0, RCM14=1
#define CHARGEDELAY 60                                                          // Seconds to wait after overcurrent, before trying again
#define BACKLIGHT 30                                                            // Seconds delay for the LCD backlight to turn off.

#define GOODFCS16 0x0f47                                                        // crc16 frame check value
#define ACK_TIMEOUT 1000                                                        // 1000ms timeout

#define STATE_A 1                                                               // Vehicle not connected
#define STATE_B 2                                                               // Vehicle connected / not ready to accept energy
#define STATE_C 3                                                               // Vehicle connected / ready to accept energy / ventilation not required
#define STATE_D 4                                                               // Vehicle connected / ready to accept energy / ventilation required
#define STATE_COMM_A 5                                                          // Waiting for response from Master
#define STATE_COMM_B 6                                                          // Waiting for response from Master
#define STATE_COMM_C 7                                                          // Waiting for response from Master
#define STATE_COMM_CB 8                                                         // Waiting for response from Master

#define STATE_CB 10

#define PILOT_12V 1
#define PILOT_9V 2
#define PILOT_6V 3
#define PILOT_DIODE 4
#define PILOT_NOK 0

#define STATE_A_TO_C PILOT_6V                                                   // Set to PILOT_6V to allow switching from STATE A to STATE C (without STATE B)
                                                                                // default is PILOT_9V

#define NO_ERROR 0
#define LESS_6A 1
#define CT_NOCOMM 2
#define TEMP_HIGH 3
#define NOCURRENT 4                                                             // No Current! ERROR=LESS_6A, switch to STATE A
#define RCD_TRIPPED 5                                                           // RCD tripped. >6mA DC residual current detected.
#define Test_IO 6

#define SOLENOID_LOCK       {LATAbits.LATA4 = 1;LATAbits.LATA5 = 0;}
#define SOLENOID_UNLOCK     {LATAbits.LATA4 = 0;LATAbits.LATA5 = 1;}
#define SOLENOID_OFF        {LATAbits.LATA4 = 1;LATAbits.LATA5 = 1;}

#define CONTACTOR_OFF LATBbits.LATB4 = 0;                                        // Contactor OFF
#define CONTACTOR_ON  LATBbits.LATB4 = 1;                                        // Contactor ON

#define BACKLIGHT_OFF LATAbits.LATA3 = 0;                                        // LCD Backlight OFF
#define BACKLIGHT_ON  LATAbits.LATA3 = 1;                                        // LCD Backlight ON

#define MENU_CONFIG 10
#define MENU_MODE 20
#define MENU_LOADBL 100
#define MENU_MAINS 30
#define MENU_MAX 40
#define MENU_MIN 50
#define MENU_LOCK 60
#define MENU_CABLE 70
#define MENU_CAL 80
#define MENU_EXIT 90
#define MENU_ACCESS 110
#define MENU_RCMON 120


#ifdef DEBUG_P
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif 

#define _RSTB_0 LATCbits.LATC4 = 0;
#define _RSTB_1 LATCbits.LATC4 = 1;
#define _A0_0 LATCbits.LATC0 = 0;
#define _A0_1 LATCbits.LATC0 = 1;


extern char GLCDbuf[256];                                                       // GLCD buffer (one row double height text only)


extern unsigned int MaxMains;                                                   // Max Mains Amps (hard limit, limited by the MAINS connection)
extern unsigned int MaxCurrent;                                                 // Max Charge current
extern unsigned int MinCurrent;                                                 // Minimal current the EV is happy with
extern double ICal;                                                             // CT calibration value
extern char Mode;                                                               // EVSE mode
extern char Lock;                                                               // Cable lock enable/disable
extern unsigned int CableLimit;                                                 // Fixed Cable Current limit (only used when config is set to Fixed Cable)
extern char Config;                                                             // Configuration (Fixed Cable or Type 2 Socket)
extern char LoadBl;                                                             // Load Balance Setting (Disable, Master or Slave)
extern char Access;                                                             // Allow access to EVSE with button on IO2
extern char RCmon;                                                              // Residual Current monitor


#define EEPROM_BYTES 19                                                         // total 19 bytes


extern double Irms[3];                                                          // Momentary current per Phase (Amps *10) (23= 2.3A)
                                                                                // Max 4 phases supported
extern unsigned int crc16;
extern unsigned char State;
extern unsigned char Error;
extern unsigned char NextState;

extern unsigned int MaxCapacity;                                                // Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
extern unsigned int Imeasured;                                                  // Max of all CT inputs (Amps *10)
extern int Balanced[4];                                                         // Amps value per EVSE (max 4)

extern unsigned char RX1byte;
extern unsigned char idx, idx2, ISRFLAG, ISR2FLAG;
extern unsigned char menu;
extern unsigned int locktimer, unlocktimer;                                     // solenoid timers
extern unsigned long Timer;                                                     // mS counter
extern unsigned int ChargeTimer;                                                // seconds counter
extern unsigned char LCDTimer;
extern unsigned char BacklightTimer;                                            // remaining seconds the LCD backlight is active
extern unsigned char TempEVSE;                                                  // Temperature EVSE in deg C (0-125)
extern unsigned char ButtonState;                                               // Holds latest push Buttons state (LSB 2:0)
extern unsigned char OldButtonState;                                            // Holds previous push Buttons state (LSB 2:0)
extern unsigned char LCDNav;
extern unsigned char SubMenu;
extern unsigned long ScrollTimer;
extern unsigned char LCDpos;
extern unsigned char ChargeDelay;                                               // Delays charging at least 60 seconds in case of not enough current available.
extern unsigned char TestState;
extern unsigned char Access_bit;

extern const far char MenuConfig[];
extern const far char MenuMode[];
extern const far char MenuLoadBl[];
extern const far char MenuMains[];
extern const far char MenuMax[];
extern const far char MenuMin[];
extern const far char MenuCable[];
extern const far char MenuLock[];
extern const far char MenuCal[];
extern const far char MenuAccess[];
extern const far char MenuRCmon[];

void delay(unsigned int d);
void read_settings(void);
void write_settings(void);

#endif