#include "CellEmulator.h"
#include "twi_master.h"
#include <Arduino.h>
#include <Wire.h>

// debug mode selects a different algorithm to test the potentiometers directly (comment out to disable)
#define DEBUG_MODE 1

CellEmulator::CellEmulator(uint8_t address, uint8_t number){
    potAddress = address;
    I2Cbus = number;
    Wire.begin();
    Serial.begin(9600);
}

CellEmulator::~CellEmulator(){}

double CellEmulator::getVoltage(){
    #ifdef DEBUG_MODE //if debug mode is enabled
        //read potmeter1 value and convert to voltage
        return (double)readWiperValue2(1);
    #endif
    return voltage;
}

int CellEmulator::setVoltage(double voltage){
    #ifdef DEBUG_MODE //if debug mode is enabled
        int value = N_STEPS * voltage/5-1;
            //write potmeter1 value
        return writeWiperValue2(1, value);
    #else //if debug mode is disabled
        //convert voltage to potmeter value
        int value = N_STEPS-(V_REF*N_STEPS/voltage);
        //write potmeter1 value
        return writeWiperValue(1, value);
    #endif
    // return value;
}

double CellEmulator::getCurrent(){
    #ifdef DEBUG_MODE
        //read potmeter0 value and convert to current
        return (double)readWiperValue2(0);
    #endif
    return current;
}

int CellEmulator::setCurrent(double value){
    #ifdef DEBUG_MODE
        uint8_t calculation = (uint8_t)((value-V_offset)/V_range*(N_STEPS-1));
        Serial.println(calculation);
        return writeWiperValue2(0, calculation);
    #else
        //convert current to potmeter value
        int value = MAX_POT_VALUE*current/MAX_CURRENT;
        //write potmeter0 value
        return writeWiperValue2(0, value);
    #endif
}

int CellEmulator::readWiperValue(uint8_t wiperSelect){
    uint8_t data[2];

    selectI2Cbus();
    
    //I2C read operation
    Wire.beginTransmission(potAddress);
    Wire.write(wiperSelect<<4|0b1100); //read command
    Wire.endTransmission();
    Wire.requestFrom(potAddress, (uint8_t)2);
    while(Wire.available()){
        Wire.readBytes(data, 2);
    }
    //return potmeter value as int16
    return ((data[0]&0x01)<<8)|data[1];
}

int CellEmulator::writeWiperValue(uint8_t wiperSelect, uint8_t value){
    selectI2Cbus();
    size_t size = 2;    
    //create data array with write command and value
    uint8_t data[size] = {(uint8_t)(wiperSelect<<4),value};
    
    //I2C write operation
    Wire.beginTransmission(potAddress);
    Wire.write(data, size);
    return Wire.endTransmission();
}

 //attempt to control the separate dig pot
int CellEmulator::writeWiperValue2(uint8_t wiperSelect, uint8_t value){
    selectI2Cbus();
    size_t size = 2;    
    //create data array with write command and value
    uint8_t data[size] = {(uint8_t)(wiperSelect<<7),value};
    
    //I2C write operation
    Wire.beginTransmission(0x2C);
    Wire.write(data, size);
    int error = Wire.endTransmission();
    return error;
}

int CellEmulator::readWiperValue2(uint8_t wiperSelect){
    int data;

    selectI2Cbus();
    
    //I2C read operation
    Wire.beginTransmission(potAddress);
    Wire.write(wiperSelect<<7); //read command
    Wire.endTransmission();
    Wire.requestFrom(potAddress, (uint8_t)1);
    while(Wire.available()){
        data = Wire.read();
    }
    //return potmeter value as int16
    return data;
}

void CellEmulator::selectI2Cbus(){
    //select correct I2C bus
    switch (I2Cbus)
    {
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