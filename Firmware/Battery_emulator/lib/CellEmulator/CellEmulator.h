#ifndef CELLEMULATOR_H
#define CELLEMULATOR_H

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define N_DATA_BYTES 2
#define I2C_BUS_SELECT_1 PD2
#define I2C_BUS_SELECT_2 PD3
#define I2C_BUS_SLAVE_NUM 7

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
#define MAX_POT_VALUE 256
#define CALCULATE_VOLTAGE(x) V_REF*N_STEPS/(N_STEPS-x)


class CellEmulator
{
public:
    //CellEmulator
    //Goal: constructor of the CellEmulator class
    //input: address (address of the cell emulator), number (number of the cell emulator)
    //output: none
    CellEmulator(int address, int number);

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
    int cellNumber = 0;
    int potAddress = 0b01010000;
    int I2Cbus = 0;

    //Goal: read the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0)
    //output: wiperValue (value of the wiper of the potmeter as int)
    int readWiperValue(int wiperSelect);

    //Goal: write the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0), value (value of the wiper of the potmeter as int)
    //Returns: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out)
    int writeWiperValue(int wiperSelect,uint8_t value);
};

//Goal: select the I2C bus to communicate with the correct cell emulator
//input: cellNumber (number of the cell emulator)
//output: none
void selectI2Cbus(int cellNumber);
#endif