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
    CellEmulator(uint8_t address, uint8_t number);
    ~CellEmulator();
    
    double getVoltage();
    void setVoltage(double voltage);

    double getCurrent();
    void setCurrent(double current);

private:
    double voltage = 2.5;
    double current = 0.1;
    uint8_t cellNumber = 0;
    uint8_t potAddress = 0b01010000;
    uint8_t I2Cbus = 0;

    uint8_t readWiperValue(uint8_t wiperSelect);
    void writeWiperValue(uint8_t wiperSelect,uint8_t value);
};

#endif

//I2C read sequence: 
//address byte;                             command byte;                                   repeated start; control byte;                               data byte;              data byte;
//fixed address;variable address;read/write //memory address(4 bit);command;    reserved(2bit)              //fixed address;variable addresss;read      //empty data(7 bit);D8  //data
//"0101"        A2 A1 A0         0(write)   //(1->4)                11(read)    XX                          //"0101"        A2 A1 A0          1(read)   //"0000000"         MSB //D7->D0 


//I2C write sequence: 
//address byte;                             command byte;                                           data byte;
//fixed address;variable address;read/write //memory address(4 bit);command;    reserved(1bit); D8  //data
//"0101"        A2 A1 A0         0(write)   //(1->4)                00(write)   X               MSB //D7->D0 
//memory address write      D8              D7->D0
