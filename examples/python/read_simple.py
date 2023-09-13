#!/usr/bin/python
import sys
import usb.core
import usb.util

#JW56FR1 defines
VID = 0x07c0    #codemercs vendor ID
PID = 0x111a    #joywarrior56fr1 product ID
DATA_SIZE = 13  #byte count for input data. 6 axes and one byte for buttons
AXE_OFFSET = 32767

dev = usb.core.find(idVendor = VID, idProduct = PID)

if dev is None:
    sys.exit('Device not found. EXIT')

try:
    dev.detach_kernel_driver(0)
except:
    pass

cfg = dev.get_active_configuration()
interface = cfg[(0,0)]

if dev.is_kernel_driver_active(interface.bInterfaceNumber):
    dev.detach_kernel_driver(interface.bInterfaceNumber)

endpoint = usb.util.find_descriptor(interface, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)

if endpoint is not None:
    try:
        while True:
            data = endpoint.read(DATA_SIZE)
           
            #get data and remove offset from joystick interface (center joytick)
            accx = (data[1] * 256 + data[0]) - AXE_OFFSET
            accy = (data[3] * 256 + data[2]) - AXE_OFFSET
            accz = (data[5] * 256 + data[4]) - AXE_OFFSET
            rx = (data[7] * 256 + data[6]) - AXE_OFFSET
            ry = (data[9] * 256 + data[8]) - AXE_OFFSET
            rz = (data[11] * 256 + data[10]) - AXE_OFFSET
            button = data[12]

            print("X: %6d    Y: %6d    Z: %6d  |  rX: %6d    rY: %6d    rZ: %6d  |  B: %02x" % (accx, accy, accz, rx, ry, rz, button))
    
    except KeyboardInterrupt: #cancel while() with CTRL+C
        sys.exit('Canceled by user')
else:
    sys.exit('Endpoint is NONE. CANCEL')
