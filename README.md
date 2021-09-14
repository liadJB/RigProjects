# RigProjects

# Nanoject II Project
## Hardware
* Mega 2560 Board
* 4X4 keypad ,based on this tutorial: https://www.circuitbasics.com/how-to-set-up-a-keypad-on-an-arduino/
* ICC 16 X2 Screen, based on this tutorial: https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
* AUX pin with wire ending (Choose Red and ground)
## Software
.ino file is the arduino script
A suitable case that fits on top of a small plastic box 
## Keypad Mapping
Colour is up to the implementer, of course.
Keypad Pin|Colour|Arduino Pin
---------|-------|-----------|
R1|White|23
R2 | Grey | 25
R3 | Purple| 27
R4 | Blue |29
C1 | Green|31
C2|Yellow |33
C3|Orange|35
C4|Brown|37

## Screen Mapping
Screen Pin|Colour|Arduino Pin
---------|-------|-----------|
Gnd| Black|Gnd
5V | Red |5V
SDA | green|SDA
SCL| Orange | SCL
## Nanoject mapping
Picked pin 49 and ground

## Script Operation
* A - starts entry of number of injetions. Press again to confirm.
* B - starts entry of number of Spacing (s). Press again to confirm.
* C - starts entry of number of Absorption Time (Min). Press again to confirm.
* D - starts injection with presented parameters. Press again to confirm
* At any point pressing # cancels whatever is happening
* '*' - turns screen and injection ability on

The user can edit the number of injections, the spacing between them, and the absorption time (essentially a timer at the end of the series of injection).
Once everything is set, one can start injection. Between each injection a timer will appear and at the end the absorption timer will countdown in seconds.

