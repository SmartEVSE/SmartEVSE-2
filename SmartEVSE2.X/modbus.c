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

#include "EVSE.h"
#include "modbus.h"
#include "utils.h"



// ########################## Modbus helper functions ##########################



/**
 * Send data over modbus
 *
 * @param unsigned char address
 * @param unsigned char function
 * @param unsigned char byte
 * @param unsigned int pointer to values
 * @param unsigned char count of values
 */
void ModbusSend(unsigned char address, unsigned char function, unsigned char byte, unsigned int *values, unsigned char count) {
    unsigned int cs, i, n = 0;
    char Tbuffer[MODBUS_BUFFER_SIZE];

    // Device address
    Tbuffer[n++] = address;
    // Function
    Tbuffer[n++] = function;
    // The number of data bytes to follow
    if (byte) Tbuffer[n++] = byte;
    // Values
    for (i = 0; i < count; i++) {
        Tbuffer[n++] = ((unsigned char)(values[i]>>8));
        Tbuffer[n++] = ((unsigned char)(values[i]));
    }
    // Calculate CRC16 from data
    cs = crc16(Tbuffer, n);
    Tbuffer[n++] = ((unsigned char)(cs));
    Tbuffer[n++] = ((unsigned char)(cs>>8));

    // Send buffer to RS485 port
    RS485SendBuf(Tbuffer, n);
}

/**
 * Send single value over modbus
 *
 * @param unsigned char address
 * @param unsigned char function
 * @param unsigned int register
 * @param unsigned int data
 */
void ModbusSend8(unsigned char address, unsigned char function, unsigned int reg, unsigned int data) {
    unsigned int values[2];

    values[0] = reg;
    values[1] = data;

    ModbusSend(address, function, 0, values, 2);
}

/**
 * Combine Bytes received over modbus
 *
 * @param pointer to var
 * @param pointer to buf
 * @param unsigned char pos
 * @param unsigned char endianness:\n
 *        0: low byte first, low word first (little endian)\n
 *        1: low byte first, high word first\n
 *        2: high byte first, low word first\n
 *        3: high byte first, high word first (big endian)
 * @param MBDataType dataType: used to determine how many bytes should be combined
 */
void combineBytes(void *var, unsigned char *buf, unsigned char pos, unsigned char endianness, MBDataType dataType) {
    char *pBytes;
    pBytes = var;

    // XC8 is little endian
    switch(endianness) {
        case ENDIANESS_LBF_LWF: // low byte first, low word first (little endian)
            *pBytes++ = (unsigned char)buf[pos + 0];
            *pBytes++ = (unsigned char)buf[pos + 1];
            if (dataType != MB_DATATYPE_INT16) {
                *pBytes++ = (unsigned char)buf[pos + 2];
                *pBytes   = (unsigned char)buf[pos + 3];
            }
            break;
        case ENDIANESS_LBF_HWF: // low byte first, high word first
            if (dataType != MB_DATATYPE_INT16) {
                *pBytes++ = (unsigned char)buf[pos + 2];
                *pBytes++ = (unsigned char)buf[pos + 3];
            }
            *pBytes++ = (unsigned char)buf[pos + 0];
            *pBytes   = (unsigned char)buf[pos + 1];
            break;
        case ENDIANESS_HBF_LWF: // high byte first, low word first
            *pBytes++ = (unsigned char)buf[pos + 1];
            *pBytes++ = (unsigned char)buf[pos + 0];
            if (dataType != MB_DATATYPE_INT16) {
                *pBytes++ = (unsigned char)buf[pos + 3];
                *pBytes   = (unsigned char)buf[pos + 2];
            }
            break;
        case ENDIANESS_HBF_HWF: // high byte first, high word first (big endian)
            if (dataType != MB_DATATYPE_INT16) {
                *pBytes++ = (unsigned char)buf[pos + 3];
                *pBytes++ = (unsigned char)buf[pos + 2];
            }
            *pBytes++ = (unsigned char)buf[pos + 1];
            *pBytes   = (unsigned char)buf[pos + 0];
            break;
        default:
            break;
    }
}



// ########################### Modbus main functions ###########################



/**
 * Request read holding (FC=3) or read input register (FC=04) to a device over modbus
 *
 * @param unsigned char address
 * @param unsigned char function
 * @param unsigned int register
 * @param unsigned int quantity
 */
void ModbusReadInputRequest(unsigned char address, unsigned char function, unsigned int reg, unsigned int quantity) {
    Modbus.RequestAddress = address;
    Modbus.RequestFunction = function;
    Modbus.RequestRegister = reg;
    ModbusSend8(address, function, reg, quantity);
}

/**
 * Response read holding (FC=3) or read input register (FC=04) to a device over modbus
 *
 * @param unsigned char address
 * @param unsigned char function
 * @param unsigned int pointer to values
 * @param unsigned char count of values
 */
void ModbusReadInputResponse(unsigned char address, unsigned char function, unsigned int *values, unsigned char count) {
    ModbusSend(address, function, count * 2u, values, count);
}

/**
 * Request write single register (FC=06) to a device over modbus
 *
 * @param unsigned char address
 * @param unsigned int register
 * @param unsigned int value
 */
void ModbusWriteSingleRequest(unsigned char address, unsigned int reg, unsigned int value) {
    Modbus.RequestAddress = address;
    Modbus.RequestFunction = 0x06;
    Modbus.RequestRegister = reg;
    ModbusSend8(address, 0x06, reg, value);
}

/**
 * Response write single register (FC=06) to a device over modbus
 *
 * @param unsigned char address
 * @param unsigned int register
 * @param unsigned int value
 */
void ModbusWriteSingleResponse(unsigned char address, unsigned int reg, unsigned int value) {
    ModbusSend8(address, 0x06, reg, value);
}


/**
 * Request write multiple register (FC=16) to a device over modbus
 *
 * @param unsigned char address
 * @param unsigned int register
 * @param unsigned char pointer to data
 * @param unsigned char count of data
 */
void ModbusWriteMultipleRequest(unsigned char address, unsigned int reg, unsigned int *values, unsigned char count) {
    unsigned int i, n = 0, cs;
    char Tbuffer[MODBUS_BUFFER_SIZE];

    Modbus.RequestAddress = address;
    Modbus.RequestFunction = 0x10;
    Modbus.RequestRegister = reg;

    // Device Address
    Tbuffer[n++] = address;
    // Function Code 16
    Tbuffer[n++] = 0x10;
    // Data Address of the first register
    Tbuffer[n++] = ((unsigned char)(reg>>8));
    Tbuffer[n++] = ((unsigned char)(reg));
    // Number of registers to write
    Tbuffer[n++] = 0x00;
    Tbuffer[n++] = count;
    // Number of data bytes to follow (2 registers x 2 bytes each = 4 bytes)
    Tbuffer[n++] = count * 2u;
    // Values
    for (i = 0; i < count; i++) {
        Tbuffer[n++] = ((unsigned char)(values[i]>>8));
        Tbuffer[n++] = ((unsigned char)(values[i]));
    }
    // Calculate CRC16 from data
    cs = crc16(Tbuffer, n);
    Tbuffer[n++] = ((unsigned char)(cs));
    Tbuffer[n++] = ((unsigned char)(cs>>8));
    // Send buffer to RS485 port
    RS485SendBuf(Tbuffer, n);
}

/**
 * Response write multiple register (FC=16) to a device over modbus
 *
 * @param unsigned char address
 * @param unsigned int register
 * @param unsigned int count
 */
void ModbusWriteMultipleResponse(unsigned char address, unsigned int reg, unsigned int count) {
    ModbusSend8(address, 0x10, reg, count);
}

/**
 * Response an exception
 *
 * @param unsigned char address
 * @param unsigned char function
 * @param unsigned char exeption
 */
void ModbusException(unsigned char address, unsigned char function, unsigned char exception) {
    unsigned int temp[1];
    ModbusSend(address, function, exception, temp, 0);
}

/**
 * Decode received modbus packet
 *
 * @param unsigned char pointer to buffer
 * @param unsigned char length of buffer
 */
void ModbusDecode(unsigned char *buf, unsigned char len) {
    // Clear old values
    Modbus.Address = 0;
    Modbus.Function = 0;
    Modbus.Register = 0;
    Modbus.RegisterCount = 0;
    Modbus.Value = 0;
    Modbus.DataLength = 0;
    Modbus.Type = MODBUS_INVALID;
    Modbus.Exception = 0;

#ifdef LOG_INFO_MODBUS
    printf("\nReceived packet");
#endif
#ifdef LOG_DEBUG_MODBUS
    printf(" (%i bytes) ", len);
    for (unsigned char x=0; x<len; x++) printf("%02x ", buf[x]);
#endif

    // Modbus error packets length is 5 bytes
    if (len == 5) {
        // calculate checksum over all data (including crc16)
        // when checksum == 0 data is ok.
        if (!crc16(buf, len)) {
            // CRC OK
            Modbus.Type = MODBUS_EXCEPTION;
            // Modbus device address
            Modbus.Address = buf[0];
            // Modbus function
            Modbus.Function = buf[1];
            // Modbus Exception code
            Modbus.Exception = buf[2];
        }
    // Modbus data packets minimum length is 8 bytes
    } else if (len >= 8) {
        // Modbus device address
        Modbus.Address = buf[0];
        // Modbus function
        Modbus.Function = buf[1];
        // calculate checksum over all data (including crc16)
        // when checksum == 0 data is ok.
        if (!crc16(buf, len)) {
            // CRC OK
#ifdef LOG_DEBUG_MODBUS
            printf("\n  valid Modbus packet: Address %02x Function %02x", Modbus.Address, Modbus.Function);
#endif
            switch (Modbus.Function) {
                case 0x03: // (Read holding register)
                case 0x04: // (Read input register)
                    if (len == 8) {
                        // request packet
                        Modbus.Type = MODBUS_REQUEST;
                        // Modbus register
                        Modbus.Register = (buf[2] <<8) | buf[3];
                        // Modbus register count
                        Modbus.RegisterCount = (buf[4] <<8) | buf[5];
                    } else {
                        // Modbus datacount
                        Modbus.DataLength = buf[2];
                        if (Modbus.DataLength == len - 5) {
                            // packet length OK
                            // response packet
                            Modbus.Type = MODBUS_RESPONSE;
#ifdef LOG_WARN_MODBUS
                        } else {
                            printf("\nInvalid modbus FC=04 packet");
#endif
                        }
                    }
                    break;
                case 0x06:
                    // (Write single register)
                    if (len == 8) {
                        // request and response packet are the same
                        Modbus.Type = MODBUS_OK;
                        // Modbus register
                        Modbus.Register = (buf[2] <<8) | buf[3];
                        // Modbus register count
                        Modbus.RegisterCount = 1;
                        // value
                        Modbus.Value = (buf[4] <<8) | buf[5];
#ifdef LOG_WARN_MODBUS
                    } else {
                        printf("\nInvalid modbus FC=06 packet");
#endif
                    }
                    break;
                case 0x10:
                    // (Write multiple register))
                    // Modbus register
                    Modbus.Register = (buf[2] <<8) | buf[3];
                    // Modbus register count
                    Modbus.RegisterCount = (buf[4] <<8) | buf[5];
                    if (len == 8) {
                        // response packet
                        Modbus.Type = MODBUS_RESPONSE;
                    } else {
                        // Modbus datacount
                        Modbus.DataLength = buf[6];
                        if (Modbus.DataLength == len - 9) {
                            // packet length OK
                            // request packet
                            Modbus.Type = MODBUS_REQUEST;
#ifdef LOG_WARN_MODBUS
                        } else {
                            printf("\nInvalid modbus FC=16 packet");
#endif
                        }
                    }
                    break;
                default:
                    break;
            }

            // Modbus.Data
            if (Modbus.Type && Modbus.DataLength) {
                // Set pointer to Data
                Modbus.Data = buf;
                // Modbus data is always at the end ahead the checksum
                Modbus.Data = Modbus.Data + (len - Modbus.DataLength - 2);
            }

            // Request - Response check
            switch (Modbus.Type) {
                case MODBUS_REQUEST:
                    Modbus.RequestAddress = Modbus.Address;
                    Modbus.RequestFunction = Modbus.Function;
                    Modbus.RequestRegister = Modbus.Register;
                    break;
                case MODBUS_RESPONSE:
                    // If address and function identical with last send or received request, it is a valid response
                    if (Modbus.Address == Modbus.RequestAddress && Modbus.Function == Modbus.RequestFunction) {
                        if (Modbus.Function == 0x03 || Modbus.Function == 0x04) 
                            Modbus.Register = Modbus.RequestRegister;
                    }
                    Modbus.RequestAddress = 0;
                    Modbus.RequestFunction = 0;
                    Modbus.RequestRegister = 0;
                    break;
                case MODBUS_OK:
                    // If address and function identical with last send or received request, it is a valid response
                    if (Modbus.Address == Modbus.RequestAddress && Modbus.Function == Modbus.RequestFunction && Modbus.Address != BROADCAST_ADR) {
                        Modbus.Type = MODBUS_RESPONSE;
                        Modbus.RequestAddress = 0;
                        Modbus.RequestFunction = 0;
                        Modbus.RequestRegister = 0;
                    } else {
                        Modbus.Type = MODBUS_REQUEST;
                        Modbus.RequestAddress = Modbus.Address;
                        Modbus.RequestFunction = Modbus.Function;
                        Modbus.RequestRegister = Modbus.Register;
                    }
                default:
                    break;
            }
        }
    }
#ifdef LOG_DEBUG_MODBUS
    if(Modbus.Type) {
        printf(" Register %04x", Modbus.Register);
    }
#endif
#ifdef LOG_INFO_MODBUS
    switch (Modbus.Type) {
        case MODBUS_REQUEST:
            printf(" Request");
            break;
        case MODBUS_RESPONSE:
            printf(" Response");
            break;
    }
#endif
}



// ########################### EVSE modbus functions ###########################


/**
 * Decode measurement value
 *
 * @param pointer to buf
 * @param unsigned char pos
 * @param unsigned char Endianness
 * @param signed char Divisor
 * @return signed long Measurement
 */
signed long receiveMeasurement(unsigned char *buf, unsigned char pos, unsigned char Endianness, MBDataType dataType, signed char Divisor) {
    signed double dCombined;
    signed long lCombined;

    if (dataType == MB_DATATYPE_FLOAT32) {
        combineBytes(&dCombined, buf, pos, Endianness, dataType);
        if (Divisor >= 0) {
            lCombined = ((signed long)dCombined) / (signed long)pow10[(unsigned)Divisor];
        } else {
            lCombined = ((signed long)dCombined) * (signed long)pow10[(unsigned)-Divisor];
        }
    } else {
        combineBytes(&lCombined, buf, pos, Endianness, dataType);
        if (dataType == MB_DATATYPE_INT16) {
            lCombined = (signed long)((int)lCombined); /* sign extend 16bit into 32bit */
        }
        if (Divisor >= 0) {
            lCombined = (signed long)(lCombined / (signed long)pow10[(unsigned)Divisor]);
        } else {
            lCombined = (signed long)(lCombined * (signed long)(pow10[(unsigned)-Divisor]));
        }
    }

    return lCombined;
}

/**
 * Send Energy measurement request over modbus
 *
 * @param unsigned char Meter
 * @param unsigned char Address
 */
void requestEnergyMeasurement(unsigned char Meter, unsigned char Address) {
    switch (Meter) {
        case EM_SOLAREDGE:
            // Note:
            // - SolarEdge uses 16-bit values, except for this measurement it uses 32bit int format
            // - EM_SOLAREDGE should not be used for EV Energy Measurements
            ModbusReadInputRequest(Address, EMConfig[Meter].Function, EMConfig[Meter].ERegister, 2);
            break;
        default:
            ModbusReadInputRequest(Address, EMConfig[Meter].Function, EMConfig[Meter].ERegister, (EMConfig[Meter].DataType == MB_DATATYPE_INT16 ? 1U : 2U));
            break;
    }
}

/**
 * Read energy measurement from modbus
 *
 * @param pointer to buf
 * @param unsigned char Meter
 * @return signed long Energy (Wh)
 */
signed long receiveEnergyMeasurement(unsigned char *buf, unsigned char Meter) {
    signed long energy;
    switch (Meter) {
        case EM_SOLAREDGE:
            // Note:
            // - SolarEdge uses 16-bit values, except for this measurement it uses 32bit int format
            // - EM_SOLAREDGE should not be used for EV Energy Measurements
            energy = receiveMeasurement(buf, 0, EMConfig[Meter].Endianness, MB_DATATYPE_INT32, EMConfig[Meter].EDivisor - 3);
            break;
        default:
            energy = receiveMeasurement(buf, 0, EMConfig[Meter].Endianness, EMConfig[Meter].DataType, EMConfig[Meter].EDivisor - 3);
            break;
    }
    return energy;
}

/**
 * Send Power measurement request over modbus
 *
 * @param unsigned char Meter
 * @param unsigned char Address
 */
void requestPowerMeasurement(unsigned char Meter, unsigned char Address) {
            ModbusReadInputRequest(Address, EMConfig[Meter].Function, EMConfig[Meter].PRegister, 2);
}

/**
 * Read Power measurement from modbus
 *
 * @param pointer to buf
 * @param unsigned char Meter
 * @return signed long Power (W)
  */
signed long receivePowerMeasurement(unsigned char *buf, unsigned char Meter) {
    signed long power;

    switch (Meter) {
        case EM_SOLAREDGE:
        {
            // Note:
            // - SolarEdge uses 16-bit values, with a extra 16-bit scaling factor
            // - EM_SOLAREDGE should not be used for EV power measurements, only PV power measurements are supported
            int scalingFactor = -(int)receiveMeasurement(buf,
                        2,
                        EMConfig[Meter].Endianness,
                        EMConfig[Meter].DataType,
                        0);
            power = receiveMeasurement(buf, 0, EMConfig[Meter].Endianness, EMConfig[Meter].DataType, scalingFactor);
            break;
        }
        default:
            power = receiveMeasurement(buf, 0, EMConfig[Meter].Endianness, EMConfig[Meter].DataType, EMConfig[Meter].PDivisor);
            break;
    }
    return power;
}

/**
 * Send current measurement request over modbus
 *
 * @param unsigned char Meter
 * @param unsigned char Address
 */
void requestCurrentMeasurement(unsigned char Meter, unsigned char Address) {
    switch(Meter) {
        case EM_SENSORBOX:
            ModbusReadInputRequest(Address, 4, 0, 20);
            break;
        case EM_EASTRON:
            // Phase 1-3 current: Register 0x06 - 0x0B (unsigned)
            // Phase 1-3 power:   Register 0x0C - 0x11 (signed)
            ModbusReadInputRequest(Address, 4, 0x06, 12);
            break;
        case EM_ABB:
            // Phase 1-3 current: Register 0x5B0C - 0x5B11 (unsigned)
            // Phase 1-3 power:   Register 0x5B16 - 0x5B1B (signed)
            ModbusReadInputRequest(Address, 3, 0x5B0C, 16);
            break;
        case EM_SOLAREDGE:
            // Read 3 Current values + scaling factor
            ModbusReadInputRequest(Address, EMConfig[Meter].Function, EMConfig[Meter].IRegister, (EMConfig[Meter].DataType == MB_DATATYPE_INT16 ? 3U : 6U) + 1U);
            break;
        default:
            // Read 3 Current values
            ModbusReadInputRequest(Address, EMConfig[Meter].Function, EMConfig[Meter].IRegister, (EMConfig[Meter].DataType == MB_DATATYPE_INT16 ? 3U : 6U));
            break;
    }
}

/**
 * Read current measurement from modbus
 *
 * @param pointer to buf
 * @param unsigned char Meter
 * @param pointer to Current (mA)
 * @return unsigned char error
 */
unsigned char receiveCurrentMeasurement(unsigned char *buf, unsigned char Meter, signed long *var) {
    unsigned char x, offset;

    // No CAL option in Menu
    CalActive = 0;

    switch(Meter) {
        case EM_SENSORBOX:
            // return immediately if the data contains no new P1 or CT measurement
            if (buf[3] == 0) return 0;  // error!!
            // determine if there is P1 data present, otherwise use CT data
            if (buf[3] & 0x80) offset = 16;                                     // P1 data present
            else offset = 28;                                                   // Use CTs
            // offset 16 is Smart meter P1 current
            for (x = 0; x < 3; x++) {
                // SmartEVSE works with Amps * 10
                var[x] = receiveMeasurement(buf,
                        offset + (x * 4U),
                        EMConfig[Meter].Endianness,
                        EMConfig[Meter].DataType,
                        EMConfig[Meter].IDivisor - 3);
                // When using CT's , adjust the measurements with calibration value
                if (offset == 28) {
                    if (x == 0) Iuncal = abs((var[x]/10));                      // Store uncalibrated CT1 measurement (10mA)
                    var[x] = (signed long)var[x] * (signed int)ICal / ICAL;
                    // When MaxMains is set to >100A, it's assumed 200A:50ma CT's are used.
                    if (MaxMains > 100) var[x] = var[x] * 2;                    // Multiply measured currents with 2
                    // very small negative currents are shown as zero.
                    if ((var[x] > -1) && (var[x] < 1)) var[x] = 0;
                    CalActive = 1;                                              // Enable CAL option in Menu
                }
            }
            // Set Sensorbox 2 to 3/4 Wire configuration (and phase Rotation) (v2.16)
            if (buf[1] >= 0x10 && offset == 28) {
                GridActive = 1;                                                 // Enable the GRID menu option
                #ifdef SPECIAL                                                  // Only when Load balancing is Disabled/Master
                if ((buf[1] & 0x3) != GRID && (LoadBl < 2)) ModbusWriteSingleRequest(0x0A, 0x800, GRID);
                #else
                if ((buf[1] & 0x3) != (Grid << 1) && (LoadBl < 2)) ModbusWriteSingleRequest(0x0A, 0x800, Grid << 1);
                #endif
            } else GridActive = 0;
            break;

        case EM_SOLAREDGE:
        {
            // Need to handle the extra scaling factor
            int scalingFactor = -(int)receiveMeasurement(buf,
                        6,
                        EMConfig[Meter].Endianness,
                        EMConfig[Meter].DataType,
                        0);
            // Now decode the three Current values using that scaling factor
            for (x = 0; x < 3; x++) {
                var[x] = receiveMeasurement(buf,
                        x * (EMConfig[Meter].DataType == MB_DATATYPE_INT16 ? 2U : 4U),
                        EMConfig[Meter].Endianness,
                        EMConfig[Meter].DataType,
                        scalingFactor - 3);
            }
            break;
        }

        default:
            for (x = 0; x < 3; x++) {
                var[x] = receiveMeasurement(buf,
                        offset + (x * (EMConfig[Meter].DataType == MB_DATATYPE_INT16 ? 2U : 4U)),
                        EMConfig[Meter].Endianness,
                        EMConfig[Meter].DataType,
                        EMConfig[Meter].IDivisor - 3);
            }
            break;
    }

    // Get sign from power measurement on some electric meters
    switch(Meter) {
        case EM_EASTRON:
            for (x = 0; x < 3; x++) {
                if (receiveMeasurement(buf, ((x + 3U) * 4U), EMConfig[Meter].Endianness, EMConfig[Meter].DataType, EMConfig[Meter].PDivisor) < 0) {
                    var[x] = -var[x];
                }
            }
            break;
        case EM_ABB:
            for (x = 0; x < 3; x++) {
                if (receiveMeasurement(buf, ((x + 5U) * 4U), EMConfig[Meter].Endianness, EMConfig[Meter].DataType, EMConfig[Meter].PDivisor) < 0) {
                    var[x] = -var[x];
                }
            }
            break;
    }

    // all OK
    return 1;
}

/**
 * Map a Modbus register to an item ID (MENU_xxx or STATUS_xxx)
 *
 * @return unsigned char ItemID
 */
unsigned char mapModbusRegister2ItemID() {
    unsigned int RegisterStart, ItemStart, Count;

    // Register 0x00*: Status
    if (Modbus.Register >= MODBUS_EVSE_STATUS_START && Modbus.Register < (MODBUS_EVSE_STATUS_START + MODBUS_EVSE_STATUS_COUNT)) {
        RegisterStart = MODBUS_EVSE_STATUS_START;
        ItemStart = STATUS_STATE;
        Count = MODBUS_EVSE_STATUS_COUNT;

    // Register 0x01*: Node specific configuration
    } else if (Modbus.Register >= MODBUS_EVSE_CONFIG_START && Modbus.Register < (MODBUS_EVSE_CONFIG_START + MODBUS_EVSE_CONFIG_COUNT)) {
        RegisterStart = MODBUS_EVSE_CONFIG_START;
        ItemStart = MENU_CONFIG;
        Count = MODBUS_EVSE_CONFIG_COUNT;

    // Register 0x02*: System configuration (same on all SmartEVSE in a LoadBalancing setup)
    } else if (Modbus.Register >= MODBUS_SYS_CONFIG_START && Modbus.Register < (MODBUS_SYS_CONFIG_START + MODBUS_SYS_CONFIG_COUNT)) {
        RegisterStart = MODBUS_SYS_CONFIG_START;
        ItemStart = MENU_MODE;
        Count = MODBUS_SYS_CONFIG_COUNT;

    } else {
        return 0;
    }

    if (Modbus.RegisterCount <= (RegisterStart + Count) - Modbus.Register) {
        return (Modbus.Register - RegisterStart + ItemStart);
    } else {
        return 0;
    }
}

/**
 * Read item values and send modbus response
 */
void ReadItemValueResponse(void) {
    unsigned char ItemID;
    unsigned char i;
    unsigned int values[MODBUS_MAX_REGISTER_READ];

    ItemID = mapModbusRegister2ItemID();
    if (ItemID) {
        for (i = 0; i < Modbus.RegisterCount; i++) {
            values[i] = getItemValue(ItemID + i);
        }
        ModbusReadInputResponse(Modbus.Address, Modbus.Function, values, Modbus.RegisterCount);
    } else {
        ModbusException(Modbus.Address, Modbus.Function, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
}

/**
 * Write item values and send modbus response
 */
void WriteItemValueResponse(void) {
    unsigned char ItemID;
    unsigned char OK = 0;

    ItemID = mapModbusRegister2ItemID();
    if (ItemID) {
        OK = setItemValue(ItemID, Modbus.Value);
    }

    if (OK && ItemID < STATUS_STATE) write_settings();

    if (Modbus.Address != BROADCAST_ADR || LoadBl == 0) {
        if (!ItemID) {
            ModbusException(Modbus.Address, Modbus.Function, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        } else if (!OK) {
            ModbusException(Modbus.Address, Modbus.Function, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        } else {
            ModbusWriteSingleResponse(Modbus.Address, Modbus.Register, Modbus.Value);
        }
    }
}

/**
 * Write multiple item values and send modbus response
 */
void WriteMultipleItemValueResponse(void) {
    unsigned char ItemID;
    unsigned int i, OK = 0, value;

    ItemID = mapModbusRegister2ItemID();
    if (ItemID) {
        for (i = 0; i < Modbus.RegisterCount; i++) {
            value = (Modbus.Data[i * 2] <<8) | Modbus.Data[(i * 2) + 1];
            OK += setItemValue(ItemID + i, value);
        }
    }

    if (OK && ItemID < STATUS_STATE) write_settings();

    if (Modbus.Address != BROADCAST_ADR || LoadBl == 0) {
        if (!ItemID) {
            ModbusException(Modbus.Address, Modbus.Function, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        } else if (!OK) {
            ModbusException(Modbus.Address, Modbus.Function, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
        } else  {
            ModbusWriteMultipleResponse(Modbus.Address, Modbus.Register, OK);
        }
    }
}
