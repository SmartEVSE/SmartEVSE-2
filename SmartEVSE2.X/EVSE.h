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


#define VERSION "2.10"                                                          // SmartEVSE software version
#define DEBUG_P                                                                 // Debug print enable/disable

#define ICAL 1.00                                                               // Irms Calibration value (for Current transformers) 
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
#define BACKLIGHT 60                                                            // Seconds delay for the LCD backlight to turn off.
#define START_CURRENT 4                                                         // Start charging when surplus current on one phase exceeds 4A (Solar)
#define STOP_TIME 10                                                            // Stop charging after 10 minutes at MIN charge current (Solar)

// Mode settings
#define MODE_NORMAL 0
#define MODE_SMART 1
#define MODE_SOLAR 2                    

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

#define NO_ERROR 0
#define LESS_6A 1
#define CT_NOCOMM 2
#define TEMP_HIGH 4
#define NOCURRENT 8                                                             // No Current! ERROR=LESS_6A, switch to STATE A
#define RCD_TRIPPED 16                                                          // RCD tripped. >6mA DC residual current detected.
#define NO_SUN 32
#define Test_IO 64

#define SOLENOID_LOCK       {PORTAbits.RA4 = 1;PORTAbits.RA5 = 0;}
#define SOLENOID_UNLOCK     {PORTAbits.RA4 = 0;PORTAbits.RA5 = 1;}
#define SOLENOID_OFF        {PORTAbits.RA4 = 1;PORTAbits.RA5 = 1;}

#define CONTACTOR_OFF PORTBbits.RB4 = 0;                                        // Contactor OFF
#define CONTACTOR_ON  PORTBbits.RB4 = 1;                                        // Contactor ON

#define BACKLIGHT_OFF PORTAbits.RA3 = 0;                                        // LCD Backlight OFF
#define BACKLIGHT_ON  PORTAbits.RA3 = 1;                                        // LCD Backlight ON

#define MENU_ENTER 1
#define MENU_CONFIG 2
#define MENU_MODE 3
#define MENU_START 4
#define MENU_STOP 5
#define MENU_LOADBL 6
#define MENU_MAINS 7
#define MENU_MIN 8
#define MENU_MAX 9
#define MENU_CABLE 10
#define MENU_LOCK 11
#define MENU_ACCESS 12
#define MENU_RCMON 13
#define MENU_CAL 14
#define MENU_EXIT 15


#ifdef DEBUG_P
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif 

#define _RSTB_0 PORTCbits.RC4 = 0;
#define _RSTB_1 PORTCbits.RC4 = 1;
#define _A0_0 PORTCbits.RC0 = 0;
#define _A0_1 PORTCbits.RC0 = 1;



//#pragma udata GLCData
extern char GLCDbuf[512];                                                       // GLCD buffer (one row double height text only)

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
extern unsigned int StartCurrent;
extern unsigned int StopTime;



extern double Irms[3];                                                          // Momentary current per Phase (Amps *10) (23= 2.3A)
                                                                                // Max 3 phases supported
extern unsigned int crc16;
extern unsigned char State;
extern unsigned char Error;
extern unsigned char NextState;

extern unsigned int MaxCapacity;                                                // Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
extern unsigned int Imeasured;                                                  // Max of all CT inputs (Amps *10)
extern int Isum;            
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

extern unsigned char MenuItems[18];

const far struct {
    char Key[7];
    char LCD[9];
    char Desc[70];
} MenuStr[21] = {
    {"",       "",         "Not in menu"},
    {"",       "",         "Hold 2 sec"},
    {"CONFIG", "CONFIG",   "Set to Fixed Cable or Type 2 Socket"},
    {"MODE",   "MODE",     "Set to Normal, Smart or Solar EVSE mode"},
    {"START",  "START",    "Surplus energy start Current"},
    {"STOP",   "STOP",     "Stop solar charging at 6A after this time"},
    {"LOADBL", "LOAD BAL", "Set Load Balancing mode"},
    {"MAINS",  "MAINS",    "Set Max MAINS Current"},
    {"MIN",    "MIN",      "Set MIN Charge Current the EV will accept"},
    {"MAX",    "MAX",      "Set MAX Charge Current for the EV"},
    {"CABLE",  "CABLE",    "Set Fixed Cable Current limit"},
    {"LOCK",   "LOCK",     "Cable locking actuator type"},
    {"ACCESS", "ACCESS",   "Access control on IO2"},
    {"RCMON",  "RCMON",    "Residual Current Monitor on IO3"},
    {"CAL",    "CAL",      "Calibrate CT1 (CT2+3 will also change)"},
    {"EXIT",   "EXIT",     "EXIT"}
};

void delay(unsigned int d);
void read_settings(void);
void write_settings(void);
unsigned char getMenuItems (void);
const char * getMenuItemOption(unsigned char nav);

#endif