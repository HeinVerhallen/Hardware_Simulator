#include "CellEmulator.h"
#include "twi_master.h"
#include <Arduino.h>
#include <Wire.h>

// debug mode selects a different algorithm to test the potentiometers directly (comment out to disable)
// #define DEBUG_MODE 1

CellEmulator::CellEmulator(uint8_t address1, uint8_t address2, uint8_t number){
    potAddress1 = address1;
    potAddress2 = address2;
    I2Cbus = number;
    Wire.begin();
}

CellEmulator::~CellEmulator(){}

double CellEmulator::getVoltage(){
    return voltage;
}

double CellEmulator::getCurrent(){
    return current;
}

double CellEmulator::getTemperature(){
    return temperature;
}

int CellEmulator::setVoltage(double value){
    #ifdef DEBUG_MODE //if debug mode is enabled
        return writeWiperValue(0, (uint8_t)value);
    #else //if debug mode is disabled
        if ((value > 5.5) | (value < V_offset)){ //input value gaurd clause
            return 7;
        }
        voltage = value;
        //write voltage value to potentiometer 2, wiper 0
        double calculation = (value < VOLTAGE_FORMULA_THRESHOLD) ? VOLTAGE_LOW_FORMULA(value) : VOLTAGE_HIGH_FORMULA(value);
        return writeWiperValue(0, (uint8_t)calculation);
    #endif
}

int CellEmulator::setCurrent(double value){
    #ifdef DEBUG_MODE
        return writeWiperValue(2, (int)value);
    #else
        if ((value > MAX_CURRENT) | (value < 0)){ //input value gaurd clause
            return 7;
        }
        current = value;
        //write current value to potentiometer 1, wiper 0
        return writeWiperValue(2, CURRENT_FORMULA(value));
    #endif
}

int CellEmulator::setTemperature(double value){
    #ifdef DEBUG_MODE
        return writeWiperValue(1, (int)value);
    #else
        if ((value >= MAX_TEMPERATURE) | (value <= MIN_TEMPERATURE)){ //input value gaurd clause
            return 7;
        }
        temperature = value;
        //write temperature value to potentiometer 1, wiper 1
        double calculation = TEMPERATURE_FORMULA(value);
        return writeWiperValue(1, (int)calculation);
    #endif
}

int CellEmulator::writeWiperValue(uint8_t wiperSelect, uint8_t value){
    selectI2Cbus();
    size_t size = 2;
    uint8_t data[size];
    data[0] = (uint8_t)(wiperSelect<<7)&0xFF;//byte 0: wiperselect 0 = 0, wiperselect 1 = 1, wiperslect 2 = 0;
    data[1] = value;//byte 1: value
    int address = (wiperSelect == 0) ? potAddress2 : potAddress1; //select correct address
    
    //I2C write operation
    Wire.beginTransmission(address);
    Wire.write(data, size);
    return Wire.endTransmission();
}

void CellEmulator::selectI2Cbus(){
    //select correct I2C bus
    switch (I2Cbus) {
    case I2C_BUS_1:
        PORTD |= (1<<I2C_BUS_SELECT_1);
        PORTD &= ~(1<<I2C_BUS_SELECT_2);
        break;
    case I2C_BUS_2:
        PORTD |= (1<<I2C_BUS_SELECT_2);
        PORTD &= ~(1<<I2C_BUS_SELECT_1);
        break;
    
    default:
        break;
    }
} 