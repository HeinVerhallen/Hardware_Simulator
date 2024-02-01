# Battery Emulator

## Table of Contents

- [Requirements](#Requirements)
- [Usage](#usage)

## Requirements

The following hardware is used:
-Arduino uno (or atmega328)
      simular avr microcontrollers will work fine with some minor adjustments.
-Cell board prototype
      no digital isolation
      2 digital potentiometers

## Usage
The firmware is written, tested and run in an VScode environment with the PlatformIO extention. Running the code another way is also possible when the source files are copied.

The 2 digital potentiometers can be addressed individually by calling the writeWiper function with wiperSelect
      - 0, meaning wiper 0 of potentiometer 1
      - 1, meaning wiper 1 of potentiometer 1
      - 2, meaning wiper 0 of potentiometer 2

The main function monitors the serial port and accepts the following commands:
      - XXXi/XXXI, update current sink to input value given by XXX in the following format: XX,XA
      - XXXv/XXXV, update output voltage to input value given by XXX in the following format: X,XXV
      - XXXt/XXXT, update output temperature to input value given by XXX in the following format: XX,X degrees.

The wiper value calculations can be adjusted by updating the function parameters or the function itself in the cellEmulator.h file.