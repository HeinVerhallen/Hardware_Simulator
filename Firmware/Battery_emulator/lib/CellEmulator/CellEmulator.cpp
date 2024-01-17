#include "CellEmulator.h"
#include "twi_master.h"
#include <Arduino.h>
#include <Wire.h>

CellEmulator::CellEmulator(int address, int number){
    potAddress = address;
    cellNumber = number;
    Wire.begin();
}

CellEmulator::~CellEmulator(){}

double CellEmulator::getVoltage(){
    #ifdef DEBUG_MODE //if debug mode is enabled
        //read potmeter1 value and convert to voltage
        return (double)(readWiperValue(1)*5)/N_STEPS;
    #else //if debug mode is disabled
        //read potmeter1 value
        return readWiperValue(1);
    #endif
}

int CellEmulator::setVoltage(double voltage){
    #ifdef DEBUG_MODE //if debug mode is enabled
        int value = N_STEPS * voltage/5-1;
    #else //if debug mode is disabled
        //convert voltage to potmeter value
        int value = N_STEPS-(V_REF*N_STEPS/voltage);
    #endif
    //write potmeter1 value
    writeWiperValue(1, value);
    return value;
}

double CellEmulator::getCurrent(){
    //read potmeter0 value and convert to current
    return (double)(MAX_CURRENT*readWiperValue(0))/MAX_POT_VALUE;
}

int CellEmulator::setCurrent(double current){
    #ifdef DEBUG_MODE
        int value = N_STEPS*current/5-1;
    #else
        //convert current to potmeter value
        int value = MAX_POT_VALUE*current/MAX_CURRENT;
    #endif
    //write potmeter0 value
    return writeWiperValue(0, value);
}

int CellEmulator::readWiperValue(int wiperSelect){
    uint8_t data[2];

    selectI2Cbus(cellNumber);
    
    //I2C read operation
    Wire.beginTransmission(potAddress);
    Wire.write(wiperSelect<<4|0b1100); //read command
    Wire.endTransmission();
    Wire.requestFrom(potAddress, 2);
    while(Wire.available()){
        Wire.readBytes(data, 2);
    }
    //return potmeter value as int16
    return ((data[0]&0x01)<<8)|data[1];
}

int CellEmulator::writeWiperValue(int wiperSelect, uint8_t value){
    selectI2Cbus(cellNumber);
    int size = 2;    
    //create data array with write command and value
    uint8_t data[size] = {(uint8_t)(wiperSelect<<4),value};
    
    //I2C write operation
    Wire.beginTransmission(potAddress);
    Wire.write(data, size);
    return Wire.endTransmission();
}

void selectI2Cbus(int cellNumber){
    //select correct I2C bus based on cell number
    if(cellNumber <= I2C_BUS_SLAVE_NUM){
        PORTD |= (1<<I2C_BUS_SELECT_1);
        PORTD &= ~(1<<I2C_BUS_SELECT_2);
    } else{
        PORTD |= (1<<I2C_BUS_SELECT_2);
        PORTD &= ~(1<<I2C_BUS_SELECT_1);
    }
}