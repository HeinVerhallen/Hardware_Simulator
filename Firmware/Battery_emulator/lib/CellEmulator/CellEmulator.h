#ifndef CELLEMULATOR_H
#define CELLEMULATOR_H

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define I2C_BUS_1 0
#define I2C_BUS_2 1
#define I2C_BUS_SELECT_1 PD2
#define I2C_BUS_SELECT_2 PD3

// Vout = 320/(256-X) where X=potentiometer wiper value(8bit) and Vout is the battery voltage (regulator parameters: Vref = 1.25V, Iadj = 50uA) 
/*Vout = Vref*Nsteps/(Nsteps-Regvalue) 
where:
Vout    = battery voltage, 
Vref    = linear regulators internal reference voltage, 
Nsteps  = number of steps the digital potentiometer has and 
x       = value of the potentiometer wiper
*/
#define V_REF 1.25
#define N_STEPS 256
#define MAX_CURRENT 13

#define V_range 3
#define V_offset 2.4


class CellEmulator
{
public:
    //CellEmulator
    //Goal: constructor of the CellEmulator class
    //input: address (address of the cell emulator), number (number of the cell emulator)
    //output: none
    CellEmulator(uint8_t address, uint8_t number);

    //CellEmulator
    //Goal: destructor of the CellEmulator class
    //input: none
    //output: none
    ~CellEmulator();
    
    //getVoltage
    //Goal: get the voltage of the cell emulator
    //input: none
    //output: voltage (voltage of the cell emulator as double)
    double getVoltage();

    //setVoltage
    //Goal: set the voltage of the cell emulator
    //input: voltage (voltage of the cell emulator as double)
    //output: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out)
    int setVoltage(double voltage);

    //getCurrent
    //Goal: get the current of the cell emulator
    //input: none
    //output: current (current of the cell emulator as double)
    double getCurrent();

    //Goal: set the current of the cell emulator
    //input: current (current of the cell emulator as double)
    //output: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out)
    int setCurrent(double current);

private:
    double voltage = 2.5;
    double current = 0.1;
    uint8_t potAddress = 0b01010000;
    uint8_t I2Cbus = 0;

    //Goal: read the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0)
    //output: wiperValue (value of the wiper of the potmeter as int)
    int readWiperValue(uint8_t wiperSelect);
      //Goal: read the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0)
    //output: wiperValue (value of the wiper of the potmeter as int)
    int readWiperValue2(uint8_t wiperSelect);

    //Goal: write the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0), value (value of the wiper of the potmeter as int)
    //Returns: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out)
    int writeWiperValue(uint8_t wiperSelect,uint8_t value);
    //Goal: write the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0), value (value of the wiper of the potmeter as int)
    //Returns: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out)
    int writeWiperValue2(uint8_t wiperSelect,uint8_t value);
    //Goal: select the I2C bus to communicate with the correct cell emulator
    //input: none
    //output: none
    void selectI2Cbus();
};
#endif