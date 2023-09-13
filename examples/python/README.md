### Requirements
To use the JoyWarrior56FR1 with python you have to install some software packages:  

```
pip install pyusb
```

Also you have to grand access to the JoyWarrior56FR1 for read and write by adding a rules file to your system. Create your own file in */etc/udev/rules.d*
with the following content:


```
KERNEL=="hidraw*", ATTRS{idVendor}=="07c0", ATTRS{idProduct}=="111a", MODE:="0666", GROUP="plugdev"
SUBSYSTEM=="usb", ATTRS{idVendor}=="07c0", ATTRS{idProduct}=="111a", MODE:="0666", GROUP="plugdev"

```

Or use the *84-joywarrior56fr1.rules* included in this repository.
To update the deivice permissions you have to reconnect the JoyWarrior56FR1.