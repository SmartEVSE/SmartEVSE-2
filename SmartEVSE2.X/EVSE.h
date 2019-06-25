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
#define SWITCH 0                                                                // 0= Charge on plugin, 1= (Push)Button on IO2 is used to Start/Stop charging.
#define RC_MON 0                                                                // Residual Current Monitoring on IO3. Disabled=0, RCM14=1
#define CHARGEDELAY 60                                                          // Seconds to wait after overcurrent, before trying again
#define BACKLIGHT 60                                                            // Seconds delay for the LCD backlight to turn off.
#define START_CURRENT 4                                                         // Start charging when surplus current on one phase exceeds 4A (Solar)
#define STOP_TIME 10                                                            // Stop charging after 10 minutes at MIN charge current (Solar)
#define MAINS_METER 2
#define MAINS_METER_ADDRESS 10
#define MAINS_METER_MEASURE 0
#define PV_METER 0
#define PV_METER_ADDRESS 11
#define EMCUSTOM_ENDIANESS 0
#define EMCUSTOM_IREGISTER 0
#define EMCUSTOM_IDIVISOR 0

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
#define MENU_LOADBL 3
#define MENU_MIN 4
#define MENU_CABLE 5
#define MENU_LOCK 6
#define MENU_START 7
#define MENU_STOP 8
#define MENU_SWITCH 9
#define MENU_RCMON 10
#define MENU_MAX 11
#define MENU_MODE 12
#define MENU_MAINS 13
#define MENU_CAL 14
#define MENU_MAINSMETER 15
#define MENU_MAINSMETERADDRESS 16
#define MENU_MAINSMETERMEASURE 17
#define MENU_PVMETER 18
#define MENU_PVMETERADDRESS 19
#define MENU_EMCUSTOM_ENDIANESS 20
#define MENU_EMCUSTOM_IREGISTER 21
#define MENU_EMCUSTOM_IDIVISOR 22
#define MENU_EXIT 23

#define STATUS_STATE 64
#define STATUS_ERROR 65
#define STATUS_MAX 66
#define STATUS_MIN 67
#define STATUS_PHASE_COUNT 68
#define STATUS_REAL_CURRENT 69
#define STATUS_CURRENT 70
#define STATUS_ACCESS 71

#define EM_SENSORBOX 1
#define EM_PHOENIX_CONTACT 2
#define EM_FINDER 3
#define EM_EASTRON 4
#define EM_CUSTOM 5

#define MODBUS_INVALID 0
#define MODBUS_OK 1
#define MODBUS_REQUEST 2
#define MODBUS_RESPONSE 3

#define MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS 0x02
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE 0x03

#ifdef DEBUG_P
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif 

#define _RSTB_0 PORTCbits.RC4 = 0;
#define _RSTB_1 PORTCbits.RC4 = 1;
#define _A0_0 PORTCbits.RC0 = 0;
#define _A0_1 PORTCbits.RC0 = 1;


extern char GLCDbuf[512];                                                       // GLCD buffer (half of the display)

extern unsigned int MaxMains;                                                   // Max Mains Amps (hard limit, limited by the MAINS connection)
extern unsigned int MaxCurrent;                                                 // Max Charge current
extern unsigned int MinCurrent;                                                 // Minimal current the EV is happy with
extern double ICal;                                                             // CT calibration value
extern char Mode;                                                               // EVSE mode
extern char Lock;                                                               // Cable lock enable/disable
extern unsigned int CableLimit;                                                 // Fixed Cable Current limit (only used when config is set to Fixed Cable)
extern char Config;                                                             // Configuration (Fixed Cable or Type 2 Socket)
extern char LoadBl;                                                             // Load Balance Setting (Disable, Master or Slave)
extern char Switch;                                                             // Allow access to EVSE with button on IO2
extern char RCmon;                                                              // Residual Current monitor
extern unsigned int StartCurrent;
extern unsigned int StopTime;
extern unsigned char MainsMeter;                                                // Type of Mains electric meter (0: Disabled / 3: sensorbox v2 / 10: Phoenix Contact / 20: Finder)
extern unsigned char MainsMeterAddress;
extern unsigned char MainsMeterMeasure;                                         // What does Mains electric meter measure (0: Mains (Home+EVSE+PV) / 1: Home+EVSE / 2: Home)
extern unsigned char PVMeter;                                                   // Type of PV electric meter (0: Disabled / 10: Phoenix Contact / 20: Finder)
extern unsigned char PVMeterAddress;
extern unsigned char EVSEMeter;                                                 // Type of EVSE electric meter (0: Disabled / 10: Phoenix Contact / 20: Finder)
extern unsigned char EVSEMeterAddress;

extern signed double Irms[3];                                                   // Momentary current per Phase (Amps *10) (23 = 2.3A)

extern unsigned char State;
extern unsigned char Error;
extern unsigned char NextState;

extern unsigned int MaxCapacity;                                                // Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
extern unsigned int Imeasured;                                                  // Max of all CT inputs (Amps * 10) (23 = 2.3A)
extern signed int Isum;
extern unsigned int Balanced[4];                                                // Amps value per EVSE (max 4)

extern unsigned char RX1byte;
extern unsigned char idx2, ISR2FLAG;
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
extern unsigned int StopTime;
extern unsigned int SolarStopTimer;

extern unsigned char MenuItems[21];

const far struct {
    char Key[7];
    char LCD[9];
    char Desc[52];
    unsigned int Min;
    unsigned int Max;
    unsigned int Default;
} MenuStr[24] = {
    {"",       "",         "Not in menu", 0, 0, 0},
    {"",       "",         "Hold 2 sec", 0, 0, 0},
    {"CONFIG", "CONFIG",   "Set to Fixed Cable or Type 2 Socket", 0, 1, CONFIG},
    {"LOADBL", "LOAD BAL", "Set Load Balancing mode", 0, 4, LOADBL},
    {"MIN",    "MIN",      "Set MIN Charge Current the EV will accept", 6, 16, MIN_CURRENT},
    {"CABLE",  "CABLE",    "Set Fixed Cable Current limit", 13, 80, CABLE_LIMIT},
    {"LOCK",   "LOCK",     "Cable locking actuator type", 0, 2, LOCK},
    {"START",  "START",    "Surplus energy start Current", 1, 16, START_CURRENT},
    {"STOP",   "STOP",     "Stop solar charging at 6A after this time", 0, 60, STOP_TIME},
    {"SW",     "SWITCH",   "Switch function control on IO2", 0, 4, SWITCH},
    {"RCMON",  "RCMON",    "Residual Current Monitor on IO3", 0, 1, RC_MON},
    {"MAX",    "MAX",      "Set MAX Charge Current for all EV", 10, 80, MAX_CURRENT},
    {"MODE",   "MODE",     "Set to Normal, Smart or Solar EVSE mode", 0, 2, MODE},
    {"MAINS",  "MAINS",    "Set Max MAINS Current", 10, 100, MAX_MAINS},
    {"CAL",    "CAL",      "Calibrate CT1 (CT2+3 will also change)", 1, 10000, (unsigned int) (ICAL * 100)},
    {"MAINEM", "MAINSMET", "Type of mains electric meter", 1, 5, MAINS_METER},
    {"MAINAD", "MAINSADR", "Address of mains electric meter", 5, 255, MAINS_METER_ADDRESS},
    {"MAINM",  "MAINSMES", "Mains electric meter scope (What does it measure?)", 0, 1, MAINS_METER_MEASURE},
    {"PVEM",   "PV METER", "Type of PV electric meter", 2, 5, PV_METER},
    {"PVAD",   "PVADDR",   "Address of PV electric meter", 5, 255, PV_METER_ADDRESS},
    {"EMBO" ,  "BYTE ORD", "Byte order of custom electric meter", 0, 3, EMCUSTOM_ENDIANESS},
    {"EMIREG", "CUR REGI", "Register for Current of custom electric meter", 0, 255, EMCUSTOM_IREGISTER},
    {"ENIDIV", "CUR DIVI", "Divisor for Current of custom electric meter", 0, 8, EMCUSTOM_IDIVISOR},
    {"EXIT",   "EXIT",     "EXIT", 0, 0, 0}
};

struct {
    unsigned char Desc[10];
    unsigned char Endianness; // 0: low byte first, low word first, 1: low byte first, high word first, 2: high byte first, low word first, 3: high byte first, high word first
    unsigned int IRegister;
    unsigned char IDivisor; // 10^x / 8:double (A)
    unsigned int ERegister;
    unsigned char ERegCount;
    unsigned char EDivisor; // 10^x / 8:double (kWh)
} EMConfig[6] = {
    {"Disabled",  0,   0, 0,     0, 0, 0}, // First entry!
    {"Sensorbox", 3,   0, 0,     0, 0, 0}, // Sensorbox (Own routine for request/receive)
    {"Phoenix C", 2, 0xC, 3,  0x3E, 2, 1}, // PHOENIX CONTACT EEM-350-D-MCB
    {"Finder",    3, 0xE, 3, 0x109, 3, 4}, // Finder 7E.78.8.400.0212
    {"Eastron",   3, 0x6, 8, 0x156, 2, 8}, // Eastron SDM630 (Own routine for request/receive)
    {"Custom",    0,   0, 0,     0, 0, 0}  // Last entry!
};

void delay(unsigned int d);
void read_settings(void);
void write_settings(void);
unsigned char getMenuItems(void);
unsigned char setItemValue(unsigned char nav, unsigned int val);
unsigned int getItemValue(unsigned char nav);
const far char * getMenuItemOption(unsigned char nav);

#endif