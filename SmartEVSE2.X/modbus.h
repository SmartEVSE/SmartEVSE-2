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

struct  {
    unsigned char Address;
    unsigned char Function;
    unsigned int Register;
    unsigned int RegisterCount;
    unsigned int Value;
    unsigned char *Data;
    unsigned char DataLength;
    unsigned char Type;
    unsigned char RequestAddress;
    unsigned char RequestFunction;
    unsigned int RequestRegister;
    unsigned char Exception;
} Modbus;

// ########################### Modbus main functions ###########################

void ModbusReadInputRequest(unsigned char address, unsigned char function, unsigned int reg, unsigned int quantity);
void ModbusReadInputResponse(unsigned char address, unsigned char function, unsigned int *values, unsigned char count);
void ModbusWriteSingleRequest(unsigned char address, unsigned int reg, unsigned int value);
void ModbusWriteSingleResponse(unsigned char address, unsigned int reg, unsigned int value);
void ModbusWriteMultipleRequest(unsigned char address, unsigned int reg, unsigned int *values, unsigned char count);
void ModbusWriteMultipleResponse(unsigned char address, unsigned int reg, unsigned int count);
void ModbusException(unsigned char address, unsigned char function, unsigned char exception);
void ModbusDecode(unsigned char *buf, unsigned char len);

// ########################### EVSE modbus functions ###########################

signed long receiveMeasurement(unsigned char *buf, unsigned char pos, unsigned char Endianness, bool IsDouble, signed char Divisor);
void requestEnergyMeasurement(unsigned char Meter, unsigned char Address);
signed long receiveEnergyMeasurement(unsigned char *buf, unsigned char Meter);
void requestPowerMeasurement(unsigned char Meter, unsigned char Address);
signed long receivePowerMeasurement(unsigned char *buf, unsigned char Meter);
void requestCurrentMeasurement(unsigned char Meter, unsigned char Address);
unsigned char receiveCurrentMeasurement(unsigned char *buf, unsigned char Meter, signed long *var);

void ReadItemValueResponse(void);
void WriteItemValueResponse(void);
void WriteMultipleItemValueResponse(void);
