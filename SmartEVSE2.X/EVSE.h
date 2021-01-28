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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "GLCD.h"

#define _XTAL_FREQ 16000000L                                                    // 16Mhz Xtal frequency

#define LOG_DEBUG 3                                                             // Debug messages including measurement data
#define LOG_INFO 2                                                              // Information messages without measurement data
#define LOG_WARN 1                                                              // Warning or error messages
#define LOG_OFF 0

#define LOG_EVSE LOG_INFO                                                       // Default: LOG_INFO
#define LOG_MODBUS LOG_WARN                                                     // Default: LOG_WARN

#define VERSION "2.20"                                                          // SmartEVSE software version
#define TRANSFORMER_COMP 100                                                    // Current calculation compensation option for use with 230V-400V transformers,
                                                                                // where the primary (MAINS) current is 1.73 times the secondary (EVSE) current.
                                                                                // set to 100 for normal use, and to 173 for use with a transformer.

//#define SPECIAL                                                               // if defined, it will modify program so that some menu options are not shown
                                                                                // should be undefined by default

#define ICAL 1024                                                               // Irms Calibration value (for Current transformers)
#define MAX_MAINS 25                                                            // max Current the Mains connection can supply
#define MAX_CURRENT 13                                                          // max charging Current for the EV
#define MIN_CURRENT 6                                                           // minimum Current the EV will accept
#define MODE 0                                                                  // Normal EVSE mode
#define LOCK 0                                                                  // No Cable lock
#define MAX_CIRCUIT 16                                                          // Max current of the EVSE circuit breaker
#define CONFIG 0                                                                // Configuration: 0= TYPE 2 socket, 1= Fixed Cable
#define LOADBL 0                                                                // Load Balancing disabled
#define SWITCH 0                                                                // 0= Charge on plugin, 1= (Push)Button on IO2 is used to Start/Stop charging.
#define RC_MON 0                                                                // Residual Current Monitoring on IO3. Disabled=0, RCM14=1
#define CHARGEDELAY 60                                                          // Seconds to wait after overcurrent, before trying again
#define BACKLIGHT 60                                                            // Seconds delay for the LCD backlight to turn off.
#define START_CURRENT 4                                                         // Start charging when surplus current on one phase exceeds 4A (Solar)
#define STOP_TIME 10                                                            // Stop charging after 10 minutes at MIN charge current (Solar)
#define IMPORT_CURRENT 0                                                        // Allow the use of grid power when solar charging (Amps)
#define MAINS_METER 1                                                           // Mains Meter, 1= Sensorbox, 2=Phoenix, 3= Finder, 4= Eastron, 5=Custom
#ifdef SPECIAL
#define GRID 1                                                                  // Grid, 0= 4-Wire CW, 1= 4-Wire CCW, 2= 3-Wire CW, 3= 3-Wire CCW
#else
#define GRID 0                                                                  // Grid, 0= 4-Wire CW, 1= 4-Wire CCW, 2= 3-Wire CW, 3= 3-Wire CCW
#endif
#define MAINS_METER_ADDRESS 10
#define MAINS_METER_MEASURE 0
#define PV_METER 0
#define PV_METER_ADDRESS 11
#define EV_METER 0
#define EV_METER_ADDRESS 12
#define MIN_METER_ADDRESS 10
#define MAX_METER_ADDRESS 247
#define EMCUSTOM_ENDIANESS 0
#define EMCUSTOM_ISDOUBLE 0
#define EMCUSTOM_UREGISTER 0
#define EMCUSTOM_UDIVISOR 8
#define EMCUSTOM_IREGISTER 0
#define EMCUSTOM_IDIVISOR 8
#define EMCUSTOM_PREGISTER 0
#define EMCUSTOM_PDIVISOR 8
#define EMCUSTOM_EREGISTER 0
#define EMCUSTOM_EDIVISOR 8
#define RFID_READER 0


// Mode settings
#define MODE_NORMAL 0
#define MODE_SMART 1
#define MODE_SOLAR 2

#define ACK_TIMEOUT 1000                                                        // 1000ms timeout
#define NR_EVSES 8
#define BROADCAST_ADR 0x09

#define STATE_A 0                                                               // A Vehicle not connected
#define STATE_B 1                                                               // B Vehicle connected / not ready to accept energy
#define STATE_C 2                                                               // C Vehicle connected / ready to accept energy / ventilation not required
#define STATE_D 3                                                               // D Vehicle connected / ready to accept energy / ventilation required (not implemented)
#define STATE_COMM_B 4                                                          // E State change request A->B (set by node)
#define STATE_COMM_B_OK 5                                                       // F State change A->B OK (set by master)
#define STATE_COMM_C 6                                                          // G State change request B->C (set by node)
#define STATE_COMM_C_OK 7                                                       // H State change B->C OK (set by master)
#define STATE_ACTSTART 8                                                        // I Activation mode in progress
#define NOSTATE 255

#define STATE_CB 10                                                             // Test code state

#define PILOT_12V 1
#define PILOT_9V 2
#define PILOT_6V 3
#define PILOT_DIODE 4
#define PILOT_NOK 0

#ifdef SPECIAL
    #define STATE_A_TO_C PILOT_6V                                               // Set to PILOT_6V to allow switching from STATE A to STATE C (without STATE B)
#else
    #define STATE_A_TO_C PILOT_9V                                               // default is PILOT_9V
#endif

#define NO_ERROR 0
#define LESS_6A 1
#define CT_NOCOMM 2
#define TEMP_HIGH 4
#define UNUSED 8                                                                // Unused
#define RCD_TRIPPED 16                                                          // RCD tripped. >6mA DC residual current detected.
#define NO_SUN 32
#define Test_IO 64
#define BL_FLASH 128

#define SOLENOID_LOCK   {LATAbits.LATA4 = 1;LATAbits.LATA5 = 0;}
#define SOLENOID_UNLOCK {LATAbits.LATA4 = 0;LATAbits.LATA5 = 1;}
#define SOLENOID_OFF    {LATAbits.LATA4 = 1;LATAbits.LATA5 = 1;}                // both outputs 12V
#define SOLENOID_0V     {LATAbits.LATA4 = 0;LATAbits.LATA5 = 0;}                // both outputs 0V

#define CONTACTOR_OFF LATBbits.LATB4 = 0;                                       // Contactor OFF
#define CONTACTOR_ON  LATBbits.LATB4 = 1;                                       // Contactor ON

#define BACKLIGHT_OFF LATAbits.LATA3 = 0;                                       // LCD Backlight OFF
#define BACKLIGHT_ON  LATAbits.LATA3 = 1;                                       // LCD Backlight ON

#define ONEWIRE_LOW     {LATBbits.LATB2 = 0;TRISBbits.TRISB2 = 0;}              // RB2 set to 0, set to output (driven low)
#define ONEWIRE_HIGH    {LATBbits.LATB2 = 1;TRISBbits.TRISB2 = 0;}              // RB2 set to 1, set to output (driven low)
#define ONEWIRE_FLOATHIGH                  {TRISBbits.TRISB2 = 1;}              // RB2 input (floating high)

#define MODBUS_INVALID 0
#define MODBUS_OK 1
#define MODBUS_REQUEST 2
#define MODBUS_RESPONSE 3
#define MODBUS_EXCEPTION 4

#define MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS 0x02
#define MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE 0x03

#define MODBUS_EVSE_STATUS_START 0xA0
#define MODBUS_EVSE_STATUS_END   0xAB
#define MODBUS_EVSE_CONFIG_START 0xC0
#define MODBUS_EVSE_CONFIG_END   0xCC
#define MODBUS_SYS_CONFIG_START  0xE0
#define MODBUS_SYS_CONFIG_END    0xF3

// EVSE status
#define STATUS_STATE 64                                                         // 0xA0: State
#define STATUS_ERROR 65                                                         // 0xA1: Error
#define STATUS_MAX 66                                                           // 0xA2: Maximum charging current
#define STATUS_MIN 67                                                           // 0xA3: Minimum charging current
#define STATUS_PHASE_COUNT 68                                                   // 0xA4: Number of used phases (ToDo)
#define STATUS_REAL_CURRENT 69                                                  // 0xA5: Real charging current (ToDo)
#define STATUS_CURRENT 70                                                       // 0xA6: Charging current (A * 10)
#define STATUS_ACCESS 71                                                        // 0xA7: Access bit
#define STATUS_MODE 72                                                          // 0xA8: EVSE Mode
#define STATUS_EVMETER 73                                                       // 0xA9: Type of EV electric meter
#define STATUS_EVMETERADDRESS 74                                                // 0xAA: Address of EV electric meter
#define STATUS_SOLAR_TIMER 75                                                   // 0xAB: Solar Timer

// EVSE configuration
#define MENU_ENTER 1
#define MENU_CONFIG 2                                                           // 0xC0: Configuration
#define MENU_LOADBL 3                                                           // 0xC1: Load Balance
#define MENU_MIN 4                                                              // 0xC2: MIN Charge Current the EV will accept
#define MENU_MAX 5                                                              // 0xC3: MAX Charge Current for this EVSE
#define MENU_LOCK 6                                                             // 0xC4: Cable lock
#define MENU_START 7                                                            // 0xC5: Surplus energy start Current
#define MENU_STOP 8                                                             // 0xC6: Stop solar charging at 6A after this time
#define MENU_SWITCH 9                                                           // 0xC7: External Start/Stop button
#define MENU_RCMON 10                                                           // 0xC8: Residual Current Monitor
#define MENU_IMPORT 11                                                          // 0xC9: Allow grid power when solar charging
#define MENU_RFIDREADER 12                                                      // 0xCA: Use RFID reader
#define MENU_EVMETER 13                                                         // 0xCB: Type of EV electric meter
#define MENU_EVMETERADDRESS 14                                                  // 0xCC: Address of EV electric meter

// System configuration (same on all SmartEVSE in a LoadBalancing setup)
#define MENU_CIRCUIT 15                                                         // 0xE0: EVSE Circuit max Current
#define MENU_MODE 16                                                            // 0xE1: EVSE mode
#define MENU_MAINS 17                                                           // 0xE2: Max Mains Current
#define MENU_CAL 18                                                             // 0xE3: CT calibration value
#define MENU_MAINSMETER 19                                                      // 0xE4: Type of Mains electric meter
#define MENU_MAINSMETERADDRESS 20                                               // 0xE5: Address of Mains electric meter
#define MENU_MAINSMETERMEASURE 21                                               // 0xE6: What does Mains electric meter measure
#define MENU_PVMETER 22                                                         // 0xE7: Type of PV electric meter
#define MENU_PVMETERADDRESS 23                                                  // 0xE8: Address of PV electric meter
#define MENU_EMCUSTOM_ENDIANESS 24                                              // 0xE9: Byte order of custom electric meter
#define MENU_EMCUSTOM_IREGISTER 25                                              // 0xEA: Register for Current (A) of custom electric meter
#define MENU_EMCUSTOM_IDIVISOR 26                                               // 0xEB: Divisor for Current (A) of custom electric meter (10^x)
#define MENU_EMCUSTOM_UREGISTER 27                                              // 0xEC: Register for Voltage (V) of custom electric meter
#define MENU_EMCUSTOM_UDIVISOR 28                                               // 0xED: Divisor for Voltage (V) of custom electric meter (10^x)
#define MENU_EMCUSTOM_PREGISTER 29                                              // 0xEE: Register for Power (W) of custom electric meter
#define MENU_EMCUSTOM_PDIVISOR 30                                               // 0xEF: Divisor for Power (W) of custom electric meter (10^x)
#define MENU_EMCUSTOM_EREGISTER 31                                              // 0xF0: Register for Energy (kWh) of custom electric meter
#define MENU_EMCUSTOM_EDIVISOR 32                                               // 0xF1: Divisor for Energy (kWh) of custom electric meter (10^x)
#define MENU_GRID 33                                                            // 0xF2: Grid type to which the Sensorbox is connected
#define MENU_EMCUSTOM_ISDOUBLE 34                                               // 0xF3: Data type of custom electric meter
#define MENU_EXIT 35

#define MENU_STATE 50

#if LOG_EVSE >= LOG_DEBUG
#define LOG_DEBUG_EVSE
#endif
#if LOG_EVSE >= LOG_INFO
#define LOG_INFO_EVSE
#endif
#if LOG_EVSE >= LOG_WARN
#define LOG_WARN_EVSE
#endif
#if LOG_MODBUS >= LOG_DEBUG
#define LOG_DEBUG_MODBUS
#endif
#if LOG_MODBUS >= LOG_INFO
#define LOG_INFO_MODBUS
#endif
#if LOG_MODBUS >= LOG_WARN
#define LOG_WARN_MODBUS
#endif

#define _RSTB_0 LATCbits.LATC4 = 0;
#define _RSTB_1 LATCbits.LATC4 = 1;
#define _A0_0 LATCbits.LATC0 = 0;
#define _A0_1 LATCbits.LATC0 = 1;

#define EM_SENSORBOX 1                                                          // Mains meter types
#define EM_PHOENIX_CONTACT 2
#define EM_FINDER 3
#define EM_EASTRON 4
#define EM_ABB 5
#define EM_CUSTOM 6

#define ENDIANESS_LBF_LWF 0
#define ENDIANESS_LBF_HWF 1
#define ENDIANESS_HBF_LWF 2
#define ENDIANESS_HBF_HWF 3

extern char GLCDbuf[512];                                                       // GLCD buffer (half of the display)

extern unsigned int MaxMains;                                                   // Max Mains Amps (hard limit, limited by the MAINS connection)
extern unsigned int MaxCurrent;                                                 // Max Charge current
extern unsigned int MinCurrent;                                                 // Minimal current the EV is happy with
extern unsigned long ICal;                                                      // CT calibration value
extern char Mode;                                                               // EVSE mode
extern char Lock;                                                               // Cable lock enable/disable
extern unsigned int MaxCircuit;                                                 // Max current of the EVSE circuit
extern char Config;                                                             // Configuration (Fixed Cable or Type 2 Socket)
extern char LoadBl;                                                             // Load Balance Setting (Disable, Master or Node)
extern char Switch;                                                             // Allow access to EVSE with button on IO2
extern char RCmon;                                                              // Residual Current monitor
extern char Grid;
extern unsigned int StartCurrent;
extern unsigned int StopTime;
extern unsigned int ImportCurrent;
extern unsigned char MainsMeter;                                                // Type of Mains electric meter (0: Disabled / Constants EM_*)
extern unsigned char MainsMeterAddress;
extern unsigned char MainsMeterMeasure;                                         // What does Mains electric meter measure (0: Mains (Home+EVSE+PV) / 1: Home+EVSE / 2: Home)
extern unsigned char PVMeter;                                                   // Type of PV electric meter (0: Disabled / Constants EM_*)
extern unsigned char PVMeterAddress;
extern unsigned char EVMeter;                                                   // Type of EV electric meter (0: Disabled / Constants EM_*)
extern unsigned char EVMeterAddress;
extern unsigned char RFIDReader;

extern signed int Irms[3];                                                      // Momentary current per Phase (Amps *10) (23 = 2.3A)

extern unsigned char State;
extern unsigned char Error;
extern unsigned char NextState;

extern unsigned int MaxCapacity;                                                // Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
extern signed int Imeasured;                                                    // Max of all CT inputs (Amps * 10) (23 = 2.3A)
extern signed int Isum;
extern unsigned int Balanced[NR_EVSES];                                         // Amps value per EVSE

extern unsigned char RX1byte;
extern unsigned char idx2, ISR2FLAG;
extern unsigned char menu;
extern unsigned long Timer;                                                     // mS counter
extern unsigned int ChargeTimer;                                                // seconds counter
extern unsigned char LCDTimer;
extern unsigned char BacklightTimer;                                            // remaining seconds the LCD backlight is active
extern signed char TempEVSE;                                                    // Temperature EVSE in deg C (0-125)
extern unsigned char ButtonState;                                               // Holds latest push Buttons state (LSB 2:0)
extern unsigned char OldButtonState;                                            // Holds previous push Buttons state (LSB 2:0)
extern unsigned char LCDNav;
extern unsigned char SubMenu;
extern unsigned long ScrollTimer;
extern unsigned char LCDpos;
extern unsigned char ChargeDelay;                                               // Delays charging at least 60 seconds in case of not enough current available.
extern unsigned char TestState;
extern unsigned char Access_bit;
extern unsigned char GridActive;                                                // When the CT's are used on Sensorbox2, it enables the GRID menu option.
extern unsigned char CalActive;                                                 // When the CT's are used on Sensorbox(1.5 or 2), it enables the CAL menu option.
extern unsigned int Iuncal;
extern unsigned int SolarStopTimer;
extern signed long EnergyCharged;
extern signed long PowerMeasured;
extern unsigned char RFID[8];
extern unsigned char RFIDstatus;

extern unsigned char MenuItems[MENU_EXIT];

const far struct {
    char Key[8];
    char LCD[9];
    char Desc[52];
    unsigned int Min;
    unsigned int Max;
    unsigned int Default;
} MenuStr[MENU_EXIT + 1] = {
    {"",       "",         "Not in menu", 0, 0, 0},
    {"",       "",         "Hold 2 sec", 0, 0, 0},
    {"CONFIG", "CONFIG",   "Set to Fixed Cable or Type 2 Socket", 0, 1, CONFIG},
    {"LOADBL", "LOAD BAL", "Set Load Balancing mode for 2-8 SmartEVSEs", 0, NR_EVSES, LOADBL},
    {"MIN",    "MIN",      "Set MIN Charge Current the EV will accept", 6, 16, MIN_CURRENT},
    {"MAX",    "MAX",      "Set MAX Charge Current for this EVSE", 6, 80, MAX_CURRENT},
    {"LOCK",   "LOCK",     "Cable locking actuator type", 0, 2, LOCK},
    {"START",  "START",    "Surplus energy start Current", 1, 16, START_CURRENT},
    {"STOP",   "STOP",     "Stop solar charging at 6A after this time", 0, 60, STOP_TIME},
    {"SW",     "SWITCH",   "Switch function control on pin SW", 0, 4, SWITCH},
    {"RCMON",  "RCMON",    "Residual Current Monitor on pin RCM", 0, 1, RC_MON},
    {"IMPORT", "IMPORT",   "Allow grid power when solar charging", 0, 6, IMPORT_CURRENT},
    {"RFID",   "RFID",     "Use RFID reader, learn/remove cards", 0, 4, RFID_READER},
    {"EVEM",   "EV METER", "Type of EV electric meter", 0, EM_CUSTOM, EV_METER},
    {"EVAD",   "EV ADDR",  "Address of EV electric meter", MIN_METER_ADDRESS, MAX_METER_ADDRESS, EV_METER_ADDRESS},
    {"CIRCUIT","CIRCUIT",  "Set EVSE Circuit max Current", 10, 160, MAX_CIRCUIT},
    {"MODE",   "MODE",     "Set to Normal, Smart or Solar EVSE mode", 0, 2, MODE},
    {"MAINS",  "MAINS",    "Set Max MAINS Current", 10, 200, MAX_MAINS},
    {"CAL",    "CAL",      "Calibrate CT1 (CT2+3 will also change)", ICAL * 0.3, ICAL * 2.0, ICAL},         // valid range is 0.3 - 2.0 times measured value
    {"MAINEM", "MAINSMET", "Type of mains electric meter", 1, EM_CUSTOM, MAINS_METER},
    {"MAINAD", "MAINSADR", "Address of mains electric meter", MIN_METER_ADDRESS, MAX_METER_ADDRESS, MAINS_METER_ADDRESS},
    {"MAINM",  "MAINSMES", "Mains electric meter scope (What does it measure?)", 0, 1, MAINS_METER_MEASURE},
    {"PVEM",   "PV METER", "Type of PV electric meter", 0, EM_CUSTOM, PV_METER},
    {"PVAD",   "PV ADDR",  "Address of PV electric meter", MIN_METER_ADDRESS, MAX_METER_ADDRESS, PV_METER_ADDRESS},
    {"EMBO" ,  "BYTE ORD", "Byte order of custom electric meter", 0, 3, EMCUSTOM_ENDIANESS},
    {"EMIREG", "CUR REGI", "Register for Current (A) of custom electric meter", 0, 65530, EMCUSTOM_IREGISTER},
    {"EMIDIV", "CUR DIVI", "Divisor for Current (A) of custom electric meter", 0, 7, EMCUSTOM_IDIVISOR},
    {"EMUREG", "VOL REGI", "Register for Voltage (V) of custom electric meter", 0, 65530, EMCUSTOM_UREGISTER},
    {"EMUDIV", "VOL DIVI", "Divisor for Voltage (V) of custom electric meter", 0, 7, EMCUSTOM_UDIVISOR},
    {"EMPREG", "POW REGI", "Register for Power (W) of custom electric meter", 0, 65534, EMCUSTOM_PREGISTER},
    {"EMPDIV", "POW DIVI", "Divisor for Power (W) of custom electric meter", 0, 7, EMCUSTOM_PDIVISOR},
    {"EMEREG", "ENE REGI", "Register for Energy (kWh) of custom electric meter", 0, 65534, EMCUSTOM_EREGISTER},
    {"EMEDIV", "ENE DIVI", "Divisor for Energy (kWh) of custom electric meter", 0, 7, EMCUSTOM_EDIVISOR},
    {"GRID",   "GRID",     "Grid type to which the Sensorbox is connected", 0, 1, GRID},
    {"EMDATA", "DATATYPE", "Data type of custom electric meter", 0, 1, EMCUSTOM_ISDOUBLE},
    {"EXIT",   "EXIT",     "EXIT", 0, 0, 0}
};

struct {
    unsigned char Desc[10];
    unsigned char Endianness; // 0: low byte first, low word first, 1: low byte first, high word first, 2: high byte first, low word first, 3: high byte first, high word first
    unsigned char Function; // 3: holding registers, 4: input registers
    bool IsDouble;
    unsigned int URegister; // Single phase voltage (V)
    unsigned char UDivisor; // 10^x
    unsigned int IRegister; // Single phase current (A)
    unsigned char IDivisor; // 10^x
    unsigned int PRegister; // Total power (W)
    unsigned char PDivisor; // 10^x
    unsigned int ERegister; // Total energy (kWh)
    unsigned char EDivisor; // 10^x
} EMConfig[EM_CUSTOM + 1] = {
    {"Disabled",  ENDIANESS_LBF_LWF, 0, false,      0, 0,      0, 0,      0, 0,      0, 0}, // First entry!
    {"Sensorbox", ENDIANESS_HBF_HWF, 4,  true, 0xFFFF, 0,      0, 0, 0xFFFF, 0, 0xFFFF, 0}, // Sensorbox (Own routine for request/receive)
    {"Phoenix C", ENDIANESS_HBF_LWF, 4, false,    0x0, 1,    0xC, 3,   0x28, 1,   0x3E, 1}, // PHOENIX CONTACT EEM-350-D-MCB (0,1V / mA / 0,1W / 0,1kWh)
    {"Finder",    ENDIANESS_HBF_HWF, 4,  true, 0x1000, 0, 0x100E, 0, 0x1026, 0, 0x1106, 3}, // Finder 7E.78.8.400.0212 (V / A / W / Wh)
    {"Eastron",   ENDIANESS_HBF_HWF, 4,  true,    0x0, 0,    0x6, 0,   0x34, 0,  0x156, 0}, // Eastron SDM630 (V / A / W / kWh)
    {"ABB",       ENDIANESS_HBF_HWF, 3, false, 0x5B00, 1, 0x5B0C, 2, 0x5B14, 2, 0x5002, 2}, // ABB B23 212-100 (0.1V / 0.01A / 0.01W / 0.01kWh) RS485 wiring reversed
    {"Custom",    ENDIANESS_LBF_LWF, 4, false,      0, 0,      0, 0,      0, 0,      0, 0}  // Last entry!
};

struct NodeStatus {
//    unsigned int State;
//    unsigned int Error;
//    unsigned int Max;
//    unsigned int Min;
//    unsigned int PhaseCount;
//    unsigned int RealCurrent;
//    unsigned int Current;
//    unsigned int Access;
//    unsigned int Mode;
    unsigned int EVMeter;
    unsigned int EVAddress;
};

void RS485SendBuf(char *buffer, unsigned char len);
void read_settings(void);
void write_settings(void);
void setSolarStopTimer(unsigned int Timer);
unsigned char getMenuItems(void);
unsigned char setItemValue(unsigned char nav, unsigned int val);
unsigned int getItemValue(unsigned char nav);
const far char * getMenuItemOption(unsigned char nav);
void WriteRFIDlist(void);

#endif