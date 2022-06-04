/*
;    Project:       Smart EVSE
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

#include <stdbool.h>

#define _XTAL_FREQ 16000000L                                                    // 16Mhz Xtal frequency

#define LOG_DEBUG 3                                                             // Debug messages including measurement data
#define LOG_INFO 2                                                              // Information messages without measurement data
#define LOG_WARN 1                                                              // Warning or error messages
#define LOG_OFF 0

#define LOG_EVSE LOG_INFO                                                       // Default: LOG_INFO
#define LOG_MODBUS LOG_WARN                                                     // Default: LOG_WARN

#define VERSION "2.30"                                                          // SmartEVSE software version
#define TRANSFORMER_COMP 100                                                    // Current calculation compensation option for use with 230V-400V transformers,
                                                                                // where the primary (MAINS) current is 1.73 times the secondary (EVSE) current.
                                                                                // set to 100 for normal use, and to 173 for use with a transformer.
#define MOD_BOOTLOADER
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
#define RFIDLOCKTIME 60                                                         // Seconds delay for the EVSE to lock again (RFIDreader = EnableOne)
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
#define EMCUSTOM_DATATYPE 0
#define EMCUSTOM_FUNCTION 4
#define EMCUSTOM_UREGISTER 0
#define EMCUSTOM_UDIVISOR 8
#define EMCUSTOM_IREGISTER 0
#define EMCUSTOM_IDIVISOR 8
#define EMCUSTOM_PREGISTER 0
#define EMCUSTOM_PDIVISOR 8
#define EMCUSTOM_EREGISTER 0
#define EMCUSTOM_EDIVISOR 8
#define RFID_READER 0
#define WIFI_MODE 0
#define AP_PASSWORD "00000000"


// Mode settings
#define MODE_NORMAL 0
#define MODE_SMART 1
#define MODE_SOLAR 2

#define ACK_TIMEOUT 1000                                                        // 1000ms timeout
#define NR_EVSES 8
#define BROADCAST_ADR 0x09
#define STARTCURRENT_AUTO_TIMER 40                                              // Automatic measure StartCurrent after 40 seconds
#define IMPORTCURRENT_ALWAYS                                                    // Import always configured current (not only on start or stop)

#define STATE_A 0                                                               // A Vehicle not connected
#define STATE_B 1                                                               // B Vehicle connected / not ready to accept energy
#define STATE_C 2                                                               // C Vehicle connected / ready to accept energy / ventilation not required
#define STATE_D 3                                                               // D Vehicle connected / ready to accept energy / ventilation required (not implemented)
#define STATE_COMM_B 4                                                          // E State change request A->B (set by node)
#define STATE_COMM_B_OK 5                                                       // F State change A->B OK (set by master)
#define STATE_COMM_C 6                                                          // G State change request B->C (set by node)
#define STATE_COMM_C_OK 7                                                       // H State change B->C OK (set by master)
#define STATE_ACTSTART 8                                                        // I Activation mode in progress
#define STATE_B1 9                                                              // J Vehicle connected / no PWM signal
#define STATE_C1 10                                                             // K Vehicle charging / no PWM signal (temp state when stopping charge from EVSE)
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

#define STATE_A_LED_BRIGHTNESS 40
#define STATE_B_LED_BRIGHTNESS 255
#define ERROR_LED_BRIGHTNESS 255
#define WAITING_LED_BRIGHTNESS 255

#define ACCESS_BUTTON 1
#define ACCESS_SWITCH 2
#define SMART_SOLAR_BUTTON 3
#define SMART_SOLAR_SWITCH 4

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

#define MODBUS_EVSE_STATUS_START 0x0000
#define MODBUS_EVSE_STATUS_COUNT 12
#define MODBUS_EVSE_CONFIG_START 0x0100
#define MODBUS_EVSE_CONFIG_COUNT 10
#define MODBUS_SYS_CONFIG_START  0x0200
#define MODBUS_SYS_CONFIG_COUNT  26

#define MODBUS_MAX_REGISTER_REQUEST 32
#define MODBUS_MAX_REGISTER_RESPONSE MODBUS_SYS_CONFIG_COUNT
#if MODBUS_MAX_REGISTER_REQUEST > MODBUS_MAX_REGISTER_RESPONSE
#define MODBUS_BUFFER_SIZE MODBUS_MAX_REGISTER_REQUEST * 2 + 10
#else
#define MODBUS_BUFFER_SIZE MODBUS_MAX_REGISTER_RESPONSE * 2 + 10
#endif

// EVSE status
#define STATUS_STATE 64                                                         // 0x0000: State
#define STATUS_ERROR 65                                                         // 0x0001: Error
#define STATUS_CURRENT 66                                                       // 0x0002: Charging current (A * 10)
#define STATUS_MODE 67                                                          // 0x0003: EVSE Mode
#define STATUS_SOLAR_TIMER 68                                                   // 0x0004: Solar Timer
#define STATUS_ACCESS 69                                                        // 0x0005: Access bit
#define STATUS_CONFIG_CHANGED 70                                                // 0x0006: Configuration changed
#define STATUS_MAX 71                                                           // 0x0007: Maximum charging current (RO)
#define STATUS_PHASE_COUNT 72                                                   // 0x0008: Number of used phases (RO)
#define STATUS_REAL_CURRENT 73                                                  // 0x0009: Real charging current (RO) (ToDo)
#define STATUS_TEMP 74                                                          // 0x000A: Temperature (RO)
#define STATUS_SERIAL 75                                                        // 0x000B: Serial number (RO)

// Node specific configuration
#define MENU_ENTER 1
#define MENU_CONFIG 2                                                           // 0x0100: Configuration
#define MENU_LOCK 3                                                             // 0x0101: Cable lock
#define MENU_MIN 4                                                              // 0x0102: MIN Charge Current the EV will accept
#define MENU_MAX 5                                                              // 0x0103: MAX Charge Current for this EVSE
#define MENU_LOADBL 6                                                           // 0x0104: Load Balance
#define MENU_SWITCH 7                                                           // 0x0105: External Start/Stop button
#define MENU_RCMON 8                                                            // 0x0106: Residual Current Monitor
#define MENU_RFIDREADER 9                                                       // 0x0107: Use RFID reader
#define MENU_EVMETER 10                                                         // 0x0108: Type of EV electric meter
#define MENU_EVMETERADDRESS 11                                                  // 0x0109: Address of EV electric meter

// System configuration (same on all SmartEVSE in a LoadBalancing setup)
#define MENU_MODE 12                                                            // 0x0200: EVSE mode
#define MENU_CIRCUIT 13                                                         // 0x0201: EVSE Circuit max Current
#define MENU_GRID 14                                                            // 0x0202: Grid type to which the Sensorbox is connected
#define MENU_CAL 15                                                             // 0x0203: CT calibration value
#define MENU_MAINS 16                                                           // 0x0204: Max Mains Current
#define MENU_START 17                                                           // 0x0205: Surplus energy start Current
#define MENU_STOP 18                                                            // 0x0206: Stop solar charging at 6A after this time
#define MENU_IMPORT 19                                                          // 0x0207: Allow grid power when solar charging
#define MENU_MAINSMETER 20                                                      // 0x0208: Type of Mains electric meter
#define MENU_MAINSMETERADDRESS 21                                               // 0x0209: Address of Mains electric meter
#define MENU_MAINSMETERMEASURE 22                                               // 0x020A: What does Mains electric meter measure
#define MENU_PVMETER 23                                                         // 0x020B: Type of PV electric meter
#define MENU_PVMETERADDRESS 24                                                  // 0x020C: Address of PV electric meter
#define MENU_EMCUSTOM_ENDIANESS 25                                              // 0x020D: Byte order of custom electric meter
#define MENU_EMCUSTOM_DATATYPE 26                                               // 0x020E: Data type of custom electric meter
#define MENU_EMCUSTOM_FUNCTION 27                                               // 0x020F: Modbus Function (3/4) of custom electric meter
#define MENU_EMCUSTOM_UREGISTER 28                                              // 0x0210: Register for Voltage (V) of custom electric meter
#define MENU_EMCUSTOM_UDIVISOR 29                                               // 0x0211: Divisor for Voltage (V) of custom electric meter (10^x)
#define MENU_EMCUSTOM_IREGISTER 30                                              // 0x0212: Register for Current (A) of custom electric meter
#define MENU_EMCUSTOM_IDIVISOR 31                                               // 0x0213: Divisor for Current (A) of custom electric meter (10^x)
#define MENU_EMCUSTOM_PREGISTER 32                                              // 0x0214: Register for Power (W) of custom electric meter
#define MENU_EMCUSTOM_PDIVISOR 33                                               // 0x0215: Divisor for Power (W) of custom electric meter (10^x)
#define MENU_EMCUSTOM_EREGISTER 34                                              // 0x0216: Register for Energy (kWh) of custom electric meter
#define MENU_EMCUSTOM_EDIVISOR 35                                               // 0x0217: Divisor for Energy (kWh) of custom electric meter (10^x)
#define MENU_EMCUSTOM_READMAX 36                                                // 0x0218: Maximum register read (ToDo)
#define MENU_WIFI 37                                                            // 0x0219: WiFi mode
#define MENU_EXIT 38

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
#define EM_SOLAREDGE 6
#define EM_WAGO 7
#define EM_CUSTOM 8

#define ENDIANESS_LBF_LWF 0
#define ENDIANESS_LBF_HWF 1
#define ENDIANESS_HBF_LWF 2
#define ENDIANESS_HBF_HWF 3

typedef enum mb_datatype {
    MB_DATATYPE_INT32 = 0,
    MB_DATATYPE_FLOAT32 = 1,
    MB_DATATYPE_INT16 = 2,
    MB_DATATYPE_MAX,
} MBDataType;

extern char GLCDbuf[512];                                                       // GLCD buffer (half of the display)

extern unsigned int MaxMains;                                                   // Max Mains Amps (hard limit, limited by the MAINS connection)
extern unsigned int MaxCurrent;                                                 // Max Charge current
extern unsigned int MinCurrent;                                                 // Minimal current the EV is happy with
extern unsigned long ICal;                                                      // CT calibration value
extern unsigned char Mode;                                                      // EVSE mode
extern char Lock;                                                               // Cable lock enable/disable
extern unsigned int MaxCircuit;                                                 // Max current of the EVSE circuit
extern char Config;                                                             // Configuration (Fixed Cable or Type 2 Socket)
extern char LoadBl;                                                             // Load Balance Setting (Disable, Master or Node)
extern char Switch;                                                             // Allow access to EVSE with button on IO2
extern char RCmon;                                                              // Residual Current monitor
extern char Grid;
extern unsigned int StartCurrent;
extern unsigned int StopTime;
extern unsigned char MainsMeter;                                                // Type of Mains electric meter (0: Disabled / Constants EM_*)
extern unsigned char MainsMeterAddress;
extern unsigned char MainsMeterMeasure;                                         // What does Mains electric meter measure (0: Mains (Home+EVSE+PV) / 1: Home+EVSE / 2: Home)
extern unsigned char PVMeter;                                                   // Type of PV electric meter (0: Disabled / Constants EM_*)
extern unsigned char PVMeterAddress;
extern unsigned char EVMeter;                                                   // Type of EV electric meter (0: Disabled / Constants EM_*)
extern unsigned char EVMeterAddress;
extern unsigned char RFIDReader;
extern unsigned char WIFImode;

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
extern char LCDpos;
extern unsigned char ChargeDelay;                                               // Delays charging at least 60 seconds in case of not enough current available.
extern unsigned char TestState;
extern unsigned char unlockMagic;
extern unsigned char unlock55;                                                  // unlock bytes set to 0 to prevent flash write at por
extern unsigned char unlockAA;                                                  // unlock bytes set to 0 to prevent flash write at por
extern unsigned char Access_bit;
extern unsigned int serialnr;
extern unsigned char GridActive;                                                // When the CT's are used on Sensorbox2, it enables the GRID menu option.
extern unsigned char CalActive;                                                 // When the CT's are used on Sensorbox(1.5 or 2), it enables the CAL menu option.
extern unsigned char SB2SoftwareVer;
extern unsigned int Iuncal;
extern unsigned int SolarStopTimer;
extern signed long EnergyCharged;
extern signed long PowerMeasured;
extern unsigned char RFIDstatus;

extern unsigned char LocalTimeSet;
extern unsigned char WiFiAPSTA;
extern unsigned char WiFiConnected;
extern unsigned char WIFImodeSB;
extern unsigned char tm_hour, tm_min;
extern unsigned char tm_mday, tm_mon;
extern unsigned char tm_year, tm_wday;
extern unsigned char SensorboxIP[4];
extern unsigned int SensorboxMAC;
extern char APpassword[];

extern unsigned char MenuItems[MENU_EXIT];

const struct {
    char Key[8];
    char LCD[9];
    char Desc[52];
    unsigned int Min;
    unsigned int Max;
    unsigned int Default;
} MenuStr[MENU_EXIT + 1] = {
    {"", "", "Not in menu", 0, 0, 0},
    {"", "", "Hold 2 sec", 0, 0, 0},

    // Node specific configuration
    /* Key,    LCD,       Desc,                                                 Min, Max, Default */
    {"CONFIG", "CONFIG",  "Fixed Cable or Type 2 Socket",                       0, 1, CONFIG},
    {"LOCK",   "LOCK",    "Cable locking actuator type",                        0, 2, LOCK},
    {"MIN",    "MIN",     "MIN Charge Current the EV will accept (per phase)",  6, 16, MIN_CURRENT},
    {"MAX",    "MAX",     "MAX Charge Current for this EVSE (per phase)",       6, 80, MAX_CURRENT},
    {"LOADBL", "LOAD BAL","Load Balancing mode for 2-8 SmartEVSEs",             0, NR_EVSES, LOADBL},
    {"SW",     "SWITCH",  "Switch function control on pin SW",                  0, 4, SWITCH},
    {"RCMON",  "RCMON",   "Residual Current Monitor on pin RCM",                0, 1, RC_MON},
    {"RFID",   "RFID",    "RFID reader, learn/remove cards",                    0, 5, RFID_READER},
    {"EVEM",   "EV METER","Type of EV electric meter",                          0, EM_CUSTOM, EV_METER},
    {"EVAD",   "EV ADDR", "Address of EV electric meter",                       MIN_METER_ADDRESS, MAX_METER_ADDRESS, EV_METER_ADDRESS},

    // System configuration
    /* Key,    LCD,       Desc,                                                 Min, Max, Default */
    {"MODE",   "MODE",    "Normal, Smart or Solar EVSE mode",                   0, 2, MODE},
    {"CIRCUIT","CIRCUIT", "EVSE Circuit max Current",                           10, 160, MAX_CIRCUIT},
    {"GRID",   "GRID",    "Grid type to which the Sensorbox is connected",      0, 1, GRID},
    {"CAL",    "CAL",     "Calibrate CT1 (CT2+3 will also change)",             (unsigned int) (ICAL * 0.3), (unsigned int) (ICAL * 2.0), ICAL}, // valid range is 0.3 - 2.0 times measured value
    {"MAINS",  "MAINS",   "Max MAINS Current (per phase)",                      10, 200, MAX_MAINS},
    {"START",  "START",   "Surplus energy start Current (sum of phases)",       0, 48, START_CURRENT},
    {"STOP",   "STOP",    "Stop solar charging at 6A after this time",          0, 60, STOP_TIME},
    {"IMPORT", "IMPORT",  "Allow grid power when solar charging (sum of phase)",0, 20, IMPORT_CURRENT},
    {"MAINEM", "MAINSMET","Type of mains electric meter",                       1, EM_CUSTOM, MAINS_METER},
    {"MAINAD", "MAINSADR","Address of mains electric meter",                    MIN_METER_ADDRESS, MAX_METER_ADDRESS, MAINS_METER_ADDRESS},
    {"MAINM",  "MAINSMES","Mains electric meter scope (What does it measure?)", 0, 1, MAINS_METER_MEASURE},
    {"PVEM",   "PV METER","Type of PV electric meter",                          0, EM_CUSTOM, PV_METER},
    {"PVAD",   "PV ADDR", "Address of PV electric meter",                       MIN_METER_ADDRESS, MAX_METER_ADDRESS, PV_METER_ADDRESS},
    {"EMBO",   "BYTE ORD","Byte order of custom electric meter",                0, 3, EMCUSTOM_ENDIANESS},
    {"EMDATA", "DATATYPE","Data type of custom electric meter",                 0, MB_DATATYPE_MAX - 1, EMCUSTOM_DATATYPE},
    {"EMFUNC", "FUNCTION","Modbus Function of custom electric meter",           3, 4, EMCUSTOM_FUNCTION},
    {"EMUREG", "VOL REGI","Register for Voltage (V) of custom electric meter",  0, 65530, EMCUSTOM_UREGISTER},
    {"EMUDIV", "VOL DIVI","Divisor for Voltage (V) of custom electric meter",   0, 7, EMCUSTOM_UDIVISOR},
    {"EMIREG", "CUR REGI","Register for Current (A) of custom electric meter",  0, 65530, EMCUSTOM_IREGISTER},
    {"EMIDIV", "CUR DIVI","Divisor for Current (A) of custom electric meter",   0, 7, EMCUSTOM_IDIVISOR},
    {"EMPREG", "POW REGI","Register for Power (W) of custom electric meter",    0, 65534, EMCUSTOM_PREGISTER},
    {"EMPDIV", "POW DIVI","Divisor for Power (W) of custom electric meter",     0, 7, EMCUSTOM_PDIVISOR},
    {"EMEREG", "ENE REGI","Register for Energy (kWh) of custom electric meter", 0, 65534, EMCUSTOM_EREGISTER},
    {"EMEDIV", "ENE DIVI","Divisor for Energy (kWh) of custom electric meter",  0, 7, EMCUSTOM_EDIVISOR},
    {"EMREAD", "READ MAX","Max register read at once of custom electric meter", 3, 255, 3},
    {"WIFI",   "WIFI",    "Connect Sensorbox-2 to WiFi access point",           0, 2, WIFI_MODE},

    {"EXIT", "EXIT", "EXIT", 0, 0, 0}
};

struct {
    unsigned char Desc[10];
    unsigned char Endianness; // 0: low byte first, low word first, 1: low byte first, high word first, 2: high byte first, low word first, 3: high byte first, high word first
    unsigned char Function; // 3: holding registers, 4: input registers
    MBDataType DataType; // How data is represented on this Modbus meter
    unsigned int URegister; // Single phase voltage (V)
    unsigned char UDivisor; // 10^x
    unsigned int IRegister; // Single phase current (A)
    unsigned char IDivisor; // 10^x
    unsigned int PRegister; // Total power (W)
    unsigned char PDivisor; // 10^x
    unsigned int ERegister; // Total energy (kWh)
    unsigned char EDivisor; // 10^x
} EMConfig[EM_CUSTOM + 1] = {
    /* Desc,      Endianness, Function, DataType,            U_Reg,Div, I_Reg,Div, P_Reg,Div, E_Reg,Div */
    {"Disabled",  ENDIANESS_LBF_LWF, 0, MB_DATATYPE_INT32,        0, 0,      0, 0,      0, 0,      0, 0}, // First entry!
    {"Sensorbox", ENDIANESS_HBF_HWF, 4, MB_DATATYPE_FLOAT32, 0xFFFF, 0,      0, 0, 0xFFFF, 0, 0xFFFF, 0}, // Sensorbox (Own routine for request/receive)
    {"Phoenix C", ENDIANESS_HBF_LWF, 4, MB_DATATYPE_INT32,      0x0, 1,    0xC, 3,   0x28, 1,   0x3E, 1}, // PHOENIX CONTACT EEM-350-D-MCB (0,1V / mA / 0,1W / 0,1kWh) max read count 11
    {"Finder",    ENDIANESS_HBF_HWF, 4, MB_DATATYPE_FLOAT32, 0x1000, 0, 0x100E, 0, 0x1026, 0, 0x1106, 3}, // Finder 7E.78.8.400.0212 (V / A / W / Wh) max read count 127
    {"Eastron",   ENDIANESS_HBF_HWF, 4, MB_DATATYPE_FLOAT32,    0x0, 0,    0x6, 0,   0x34, 0,  0x156, 0}, // Eastron SDM630 (V / A / W / kWh) max read count 80
    {"ABB",       ENDIANESS_HBF_HWF, 3, MB_DATATYPE_INT32,   0x5B00, 1, 0x5B0C, 2, 0x5B14, 2, 0x5002, 2}, // ABB B23 212-100 (0.1V / 0.01A / 0.01W / 0.01kWh) RS485 wiring reversed / max read count 125
    {"SolarEdge", ENDIANESS_HBF_HWF, 3, MB_DATATYPE_INT16,    40196, 0,  40191, 0,  40083, 0,  40226, 3}, // SolarEdge SunSpec (0.01V (16bit) / 0.1A (16bit) / 1W (16bit) / 1 Wh (32bit))
    {"WAGO",      ENDIANESS_HBF_HWF, 3, MB_DATATYPE_FLOAT32, 0x5002, 0, 0x500C, 0, 0x5012,-3, 0x6000, 0}, // WAGO 879-30x0 (V / A / kW / kWh)
    {"Custom",    ENDIANESS_LBF_LWF, 4, MB_DATATYPE_INT32,        0, 0,      0, 0,      0, 0,      0, 0}  // Last entry!
};

struct NodeStatus {
    bool Online;
    unsigned char ConfigChanged;
    unsigned char EVMeter;
    unsigned char EVAddress;
    unsigned char MinCurrent; // 0.1A
    unsigned char Phases;
    unsigned int Timer; // 1s
} Node[NR_EVSES];

void RS485SendBuf(char *buffer, unsigned char len);
void eeprom_read_object(void *obj_p, size_t obj_size);
void eeprom_write_object(void *obj_p, size_t obj_size);
void read_settings(void);
void write_settings(void);
void setSolarStopTimer(unsigned int Timer);
void setState(unsigned char NewState);
void setAccess(bool Access);
unsigned int getSerialNr(void);
unsigned char getMenuItems(void);
unsigned char setItemValue(unsigned char nav, unsigned int val);
unsigned int getItemValue(unsigned char nav);
const char * getMenuItemOption(unsigned char nav);

#endif
