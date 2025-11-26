#!/usr/bin/python
import sys
import usb.core
import usb.util
import struct

# Device VendorID and ProductID
VID = 0x07c0
PID = 0x111a

# Set the Joystick axis to '0'
AXE_OFFSET = 32767

# Interfaces
INTERFACE_NORMAL = 0
INTERFACE_ACCEL = 1
INTERFACE_ROT = 2

# Device modes
MODE_NORMAL = 0
MODE_HIGHSPEED = 1

#Parameter
ARNG = 0x00 #default
AFILT = 0x00 #default
GRNG = 0x00 #default
GFILT = 0x01 #default
MODE = MODE_HIGHSPEED

dev = usb.core.find(idVendor=VID, idProduct=PID)
if dev is None:
    sys.exit('Device not found')

# Beide Interfaces vom Kernel lösen
for iface in [INTERFACE_NORMAL, INTERFACE_ACCEL]:
    try:
        dev.detach_kernel_driver(iface)
    except Exception:
        pass

cfg = dev.get_active_configuration()

# Interface 0: OUT-Endpoint (0x02)
interface0 = cfg[(INTERFACE_NORMAL, 0)]
ep_out = usb.util.find_descriptor(interface0, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT)

# Interface 1: IN-Endpoint (0x83)
interface1 = cfg[(INTERFACE_ACCEL, 0)]
ep_accel_in = usb.util.find_descriptor(interface1, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)

# Interface 2: IN-Endpoint (0x84)
#interface2 = cfg[(INTERFACE_ROT, 0)]
#ep_rotation_in = usb.util.find_descriptor(interface2, custom_match=lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_IN)

# Send configuration
activate_cmd = bytes([0x00, ARNG, AFILT, GRNG, GFILT, 0x00, 0x00, 0x00])
bytes_written = ep_out.write(activate_cmd)

# Activate highspeed interface
activate_cmd = bytes([0x04, MODE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
bytes_written = ep_out.write(activate_cmd)

# Read data from device
try:
    while True:

        data = ep_accel_in.read(ep_accel_in.wMaxPacketSize, timeout = 0)
        data_raw = struct.unpack('<30H', data[:60])  # H = unsigned, last 2 Bytes of data are temperatur values

        for i in range(0, 30, 3):
            x = data_raw[i] - AXE_OFFSET
            y = data_raw[i+1] - AXE_OFFSET
            z = data_raw[i+2] - AXE_OFFSET
            print(f"Set {i//3}: X={x:6d}  Y={y:6d}  Z={z:6d}")
        
        # Temperatur (last 2 Bytes of data)
        # temperatur = struct.unpack('<H', data[60:62])[0]
        # print(f"Temperatur: {temperatur}")
      

except KeyboardInterrupt:
    print("Exit")

finally:
    # Restore JW56FR1 to normal behavior
    deactivate_cmd = bytes([0x04, MODE_NORMAL, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00])
    ep_out.write(deactivate_cmd)
    
    # Free USB-Ressourcen
    usb.util.dispose_resources(dev)
    
    # Kernel-Driver reattach
    for iface in [INTERFACE_NORMAL, INTERFACE_ACCEL]:
        try:
            dev.attach_kernel_driver(iface)
        except Exception:
            pass
    
    print("Cleanup complete")