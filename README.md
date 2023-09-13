# JoyWarrior56FR1 - Examples to use the JW56FR1 on Linux based systems
Example code for using the JoyWarrior56FR1 on linux based systems by using various APIs and programming languages.


## What is a JoyWarrior56FR1
The JoyWarrior56FR1 is an acceleration sensor with an USB interface, with 6 axis (acceleration and angular rate), and 8 digital inputs. It may be used for motion, vibration, or orientation sensing.
For more detailed information please visit the [product page](https://www.codemercs.com/en/joystick/acceleration).


## Output format
The JoyWarrior56FR1 will send 13 bytes of data to the host every 2ms. The resolution for each axis is 16 bit, the data format will be LSB first.

| B0 + B1 | B2 + B3 | B4 + B5 | B6 + B7 | B8 + B9 | B10 + B11 | B12
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| X  | Y | Z | rX | rY | rZ | 8bit input

To get the 16 Bit value using the following formula: 
```
X = (B1 << 8) | B0
```
Because the JoyWarrior56FR1 is a joystick device, the data will have an offset of 0x7FFF (32767) to center the axis. To get an positiv and negativ result you have to decrease the axe value by 0x7FFF each.  


# Sensor parameters
The JoyWarrior56FR1 can be configured with some parameters. For more information about the parameters please take a look into the [Datasheet](https://www.codemercs.com/downloads/joywarrior/JW56FR1_Datasheet.pdf). 


# Using 3rd party API and toolchains
For some of the example projecte we are using 3rd party software and API like LibUSB. Please take care about the license about each API.


### LibUSB
We provide some code examples for the JoyWarrior56FR1 by using the [LibUSB API](https://libusb.info/) for cummunication. You can find the repository [here](https://github.com/libusb/libusb).


## Test hardware (host system)
For our examples on Linux we are using a Raspberry PI for development and testing. The examples should work on a x86 or x64 based system also. If you are using the 3rd party APIs please follow the install instructions of the specific API.


## Read/Write permissions for JoyWarrior56FR1
To access the JoyWarrior56FR1, some API need access to the JoyWarrior56FR1.
For this you have to add a rules files in */etc/udev/rules.d* to grand access for users. Copy the *84-joywarrior56fr1.rules* file to */etc/udev/rules.d*.


## Links and further information
[Product site](https://www.codemercs.com/en/joystick/acceleration) for more informations, datasheets, and software/tools for the JoyWarrior56FR1 devices  
[Company site](https://www.codemercs.com) for information on more devices.


## Contact
If you have any questions about the IO-Warrior please contact **support@codemercs.com** or using the [issues](https://github.com/codemercs-com/jw56fr1-linux/issues) section in this repository. There is also a company [forum](https://forum.codemercs.com/) with many solved questions.
