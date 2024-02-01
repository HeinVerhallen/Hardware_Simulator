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
// #define V_REF 1.25
// #define V_range 2.991
#define V_offset 2.514

#define VOLTAGE_LOW_FORMULA(value) (value-2.5143)/0.0166
#define VOLTAGE_FORMULA_THRESHOLD 5.44
#define VOLTAGE_HIGH_FORMULA(value) 27031*value*value-294887*value+804432

#define N_STEPS 256
#define MAX_CURRENT 13
#define MIN_CURRENT 0
#define CURRENT_FORMULA(value) N_STEPS-1-((value-MIN_CURRENT)/(MAX_CURRENT-MIN_CURRENT)*N_STEPS-1)

#define MAX_TEMPERATURE 100
#define MIN_TEMPERATURE 0
#define TEMPERATURE_FORMULA(value) (value-MIN_TEMPERATURE)/(MAX_TEMPERATURE-MIN_TEMPERATURE)*N_STEPS-1


class CellEmulator
{
public:
    //CellEmulator
    //Goal: constructor of the CellEmulator class
    //input: address (address of the cell emulator), number (number of the cell emulator)
    //output: none
    CellEmulator(uint8_t address1, uint8_t address2, uint8_t number);

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
    //output: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out, 7 = value out of range)
    int setVoltage(double voltage);

    //getCurrent
    //Goal: get the current of the cell emulator
    //input: none
    //output: current (current of the cell emulator as double)
    double getCurrent();

    //setCurrent
    //Goal: set the current of the cell emulator
    //input: current (current of the cell emulator as double)
    //output: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out, 7 = value out of range)
    int setCurrent(double current);

    //getTemperature
    //Goal: get the temperature of the cell emulator
    //input: none
    //output: temperature (temperature of the cell emulator as double)
    double getTemperature();

    //setTemperature
    //Goal: set the temperature of the cell emulator
    //input: temperature (temperature of the cell emulator as double)
    //output: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out, 7 = value out of range)
    int setTemperature(double temperature);

private:
    double voltage = 0.0;
    double current = 0.0;
    double temperature = 0;
    uint8_t potAddress1 = 0b01010000;
    uint8_t potAddress2 = 0b01011000;
    uint8_t I2Cbus = 0;

    //Goal: write the value of the wiper of the potmeter
    //input: wiperSelect (1 = potmeter1, 0 = potmeter0), value (value of the wiper of the potmeter as int)
    //Returns: I2C transmission status (0 = success, 1 = data too long to fit in transmit buffer, 2 = received NACK on transmit of address, 3 = received NACK on transmit of data, 4 = other error, 5 = time out)
    int writeWiperValue(uint8_t wiperSelect,uint8_t value);
    //Goal: select the I2C bus to communicate with the correct cell emulator
    //input: none
    //output: none
    void selectI2Cbus();
};
#endif