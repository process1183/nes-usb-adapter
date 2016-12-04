# nes-usb-adapter #

This project allows you to use an original NES controller with a PC. It uses 
the [Teensy 2.0](http://pjrc.com/teensy/index.html) USB Joystick functionality 
to accomplish this. This works great with [FCEUX](http://www.fceux.com/web/) 
and [SuperTux](https://supertuxproject.org/).

The NES controller uses a 4021 8-bit shift register, which is easily interfaced 
with the Teensy (see the [Arduino ShiftIn Tutorial](http://www.arduino.cc/en/Tutorial/ShiftIn)).

## NES Controller Connector Pinout ##
      ____
    1 |o  \
    2 |o  o| 5
    3 |o  o| 6
    4 |o  o| 7
      ------
    
    1. GND
    2. CLK
    3. Latch
    4. Data
    5. VCC
    6. NC
    7. NC


## Joystick Button Mapping ##

NES Controller | Joystick/Gamepad
---------------|-----------------
Left | 1
Right | 2
Down | 3
Up | 4
Start | 5
Select | 6
B | 7
A | 8
