SmartEVSE
=========

Smart Electric Vehicle Charge Controller

![Image of SmartEVSE](/pictures/SmartEVSEv2.jpg)

# What is it?

It's a open source EVSE (Electric Vehicle Supply Equipment). It supports 1-3 phase charging, fixed charging cable or charging socket. Locking actuator support (3 different types). And it can directly drive a mains contactor for supplying power to the EV. It features a display from which all module parameters can be configured.<br>
Up to 8 modules can be connected together to charge up to eight EV's from one mains connection without overloading it.<br>
The mains connection can be monitored by the (optional) sensorbox or a modbus kWh meter. This allows smart charging.
Communication between the SmartEVSE(s) / Sensorbox or kWh meters is done over RS485(modbus).

The project consists of two parts:
- Smart EVSE controller
- Sensorbox

# Features

- Fits into a standard DIN rail enclosure.
- Measures the current consumption of other appliances, and automatically lowers or increases the charging current to the EV. (sensorbox required)
- The load balancing feature let's you connect up to 8 SmartEVSE's to one mains supply.
- Switched 230VAC output, for contactor/relay.
- Powered RS485 communication bus for sensorbox / Modbus kWh Meters.
- Can be used with fixed cable, or socket and charging cable.
- Automatically selects current capacity of the connected cable (13/16/32A)
- Locking actuator support, locks the charging cable in the socket, automatically unlocks on a power failure.
- RFID reader support, restrict the use of the charging station to max 20 RFID cards.
- An optional modbus kWh meter will measure power and charged energy, and display this on the LCD.
- Built-in temperature sensor.
- All module parameters can be configured using the display and buttons.
- Setup can also be done through serial CLI.
- Firmware upgradable through serial bootloader. 

New V2.20 features are:
- Added EVMETER and EVADR option to the menu. Use a modbus kWh meter to measure the charged energy.
- Added charged energy to the LCD (Smart/Solar mode)
- Uses condensed characters on the LCD, so the 'i' will not use the same space as the 'w'
- Increased the nr of nodes to 7.
- Added RFID reader option. Learn up to 20 RFID cards.

# The Sensorbox

The Sensorbox should be placed where the Mains connection enters the building. Usually just after the kWh meter, this way it will be able to measure the total current per phase and send this information to the SmartEVSE.

In order to measure the current, Current transformers are used. Clip them around the L1,L2 and L3 wires, and plug the other end into the sensorbox.

![Image of Sensorbox](/pictures/sensorbox.jpg)

# Configuring the SmartEVSE

The SmartEVSE has a display, which shows the charging status, and if smart mode is used, also the measured current per phase.
It is possible to configure all settings using the built-in menu.

Three buttons below the display are used for navigating the menu.

Hold the center button for 2 seconds to enter the menu.
You can now use the left and right buttons to go to the different menu options. 

Pressing the center button, selects the option, and allows you change the value (for example change the charging current)

![Image of Menu](/pictures/SmartEVSEv2_mode_smart.jpg)

# Building the EVSE

In order to build a complete EVSE (charging station)
you will need:

- SmartEVSE.
- 4 pole NO Contactor rated for the max charging current. (for example Hager ESC440 or ISKRA IKA432-40)
- Fixed charging cable or socket with locking solenoid.
- Enclosure with DIN rail. 
- Terminal blocks (Wago TOPJOB S)

The EVSE needs to be protected with a circuit breaker and residual-current circuit breaker, usually located near or in the distribution board.

# Purchase the SmartEVSE

You can buy a complete assembled SmartEVSEv2 [here](http://www.stegen.com/en/ev-products/66-smart-evse-controller.html)<br>
Information on how to compile and setup the controller can be found on the [smartevse.nl](http://www.smartevse.nl) website.

**The assembled SmartEVSE v2 modules come pre-programmed with the latest firmware (v2.20), and are fully tested!**

# Predefined electric meters

- Sensorbox (1)
- PHOENIX CONTACT EEM-350-D-MCB (2)
- Finder 7E.78.8.400.0212 (3)
- Eastron SDM630 (4)
- ABB B23 212-100 (5)
- Custom (6)

# Modbus

Baudrate is 9600bps 8N1 (8bit, no parity bit, one stop bit).
All registers are 16 bit unsigned integers.
Address depends on the Load Balancing configuration:

Load Balancing | Modbus Address
--- | ---
Disabled/Master | 0x01
Node 1 | 0x02
Node 2 | 0x03
Node 3 | 0x04
Node 4 | 0x05
Node 5 | 0x06
Node 6 | 0x07
Node 7 | 0x08

Broadcast to all SmartEVSE with address 0x09.

## Register 0x000*: Broadcast Chargecurrent or Error

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x01 | W | Broadcast ChargeCurrent | 0.1 A | Charge Current (0 A = no current available)
0x02 | W | Broadcast Error | Bit | 1:LESS_6A / 2:NO_COMM / 4:TEMP_HIGH / 8:NO_CURRENT / 16:RCD / 32:NO_SUN

Register 0x01 is written to every 2 seconds by the Master, and holds the Charge Current per Node EVSE. A total of 4 words (8 bytes) are written, One word per EVSE(0-3).<br>
Register 0x02 is written to only if an error occurred.<br>
Use function code 0x10 (Preset Multiple Registers), and broadcast address 0x09 to use this feature.<br>

## Register 0x00*: EVSE status

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x0000 | R/W | State | | A-D (EVSE State), E-H (A-D Waiting for Balance Master)
0x0001 | R/W | Error | Bit | 1:LESS_6A / 2:NO_COMM / 4:TEMP_HIGH / 8:Unused / 16:RCD / 32:NO_SUN
0x0002 | R/W | Charging current | 0.1 A | 0:no current available / 6-80
0x0003 | R/W | EVSE mode (without saving) | | 0:Normal / 1:Smart / 2:Solar
0x0004 | R/W | Solar Timer | s |
0x0005 | R/W | Access bit | | 0:No Access / 1:Access
0x0006 | R/W | Configuration changed (Not implemented) | |
0x0007 | R | Maximum charging current | A |
0x0008 | R | Number of used phases (Not implemented) | | 0:Undetected / 1 - 3
0x0009 | R | Real charging current (Not implemented) | 0.1 A |
0x000A | R | Temperature | K |
0x000B | R | Serial number | |

## Register 0x01*: Node specific configuration

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x0100 | R/W | Configuration | | 0:Socket / 1:Fixed Cable
0x0101 | R/W | Cable lock | | 0:Disable / 1:Solenoid / 2:Motor
0x0102 | R/W | MIN Charge Current the EV will accept | A | 6 - 16
0x0103 | R/W | MAX Charge Current for this EVSE | A | 6 - 80
0x0104 | R/W | Load Balance | | 0:Disabled / 1:Master / 2-8:Node
0x0105 | R/W | External Switch on pin SW | | 0:Disabled / 1:Access Push-Button / 2:Access Switch / 3:Smart-Solar Push-Button / 4:Smart-Solar Switch
0x0106 | R/W | Residual Current Monitor on pin RCM | | 0:Disabled / 1:Enabled
0x0107 | R/W | Use RFID reader | | 0:Disabled / 1:Enabled
0x0108 | R/W | Type of EV electric meter | | *
0x0109 | R/W | Address of EV electric meter | | 10 - 247

## Register 0x02*: System configuration (same on all SmartEVSE in a LoadBalancing setup)

Register | Access | Description | Unit | Values
--- | --- | --- | --- | ---
0x0200 | R/W | EVSE mode | | 0:Normal / 1:Smart / 2:Solar
0x0201 | R/W | EVSE Circuit max Current | A | 10 - 160
0x0202 | R/W | Grid type to which the Sensorbox is connected | | 0:4Wire / 1:3Wire
0x0203 | R/W | CT calibration value | 0.01 | Multiplier
0x0204 | R/W | Max Mains Current | A | 10 - 200
0x0205 | R/W | Surplus energy start Current | A | 1 - 16
0x0206 | R/W | Stop solar charging at 6A after this time | min | 0:Disable / 1 - 60
0x0207 | R/W | Allow grid power when solar charging | A | 0 - 6
0x0208 | R/W | Type of Mains electric meter | | *
0x0209 | R/W | Address of Mains electric meter | | 10 - 247
0x020A | R/W | What does Mains electric meter measure | | 0:Mains (Home+EVSE+PV) / 1:Home+EVSE
0x020B | R/W | Type of PV electric meter | | *
0x020C | R/W | Address of PV electric meter | | 10 - 247
0x020D | R/W | Byte order of custom electric meter | | 0:LBF & LWF / 1:LBF &  HWF / 2:HBF & LWF / 3:HBF & HWF
0x020E | R/W | Data type of custom electric meter | | 0:Integer / 1:Double
0x020F | R/W | Maximum register read (Not implemented) | |
0x0210 | R/W | Register for Voltage (V) of custom electric meter | | 0 - 65530
0x0211 | R/W | Divisor for Voltage (V) of custom electric meter | 10<sup>x</sup> | 0 - 7
0x0212 | R/W | Register for Current (A) of custom electric meter | | 0 - 65530
0x0213 | R/W | Divisor for Current (A) of custom electric meter | 10<sup>x</sup> | 0 - 7
0x0214 | R/W | Register for Power (W) of custom electric meter | | 0 - 65534
0x0215 | R/W | Divisor for Power (W) of custom electric meter | 10<sup>x</sup> | 0 - 7 /
0x0216 | R/W | Register for Energy (kWh) of custom electric meter | | 0 - 65534
0x0217 | R/W | Divisor for Energy (kWh) of custom electric meter | 10<sup>x</sup> | 0 - 7

 * Number in brackets in section "Predefined electric meters"
