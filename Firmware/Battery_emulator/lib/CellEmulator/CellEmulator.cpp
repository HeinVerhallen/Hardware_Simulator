#include "CellEmulator.h"
// #include "twi_master.h"
#include "twi_master.h"
#include <Arduino.h>
#include <Wire.h>

uint8_t pullen = 0;

CellEmulator::CellEmulator(int address, int number){
    potAddress = address;
    cellNumber = number;
    Wire.begin();
}

CellEmulator::~CellEmulator(){}

double CellEmulator::getVoltage(){
    //read potmeter1 value
    // return (double)V_REF*N_STEPS/(N_STEPS-readWiperValue(1));
    return (double)(readWiperValue(1)*5)/N_STEPS;
}

int CellEmulator::setVoltage(double voltage){
    // int value = N_STEPS-(V_REF*N_STEPS/voltage);
    int value = N_STEPS * voltage/5-1;
    return writeWiperValue(1, value);
    //write potmeter1 value
}

double CellEmulator::getCurrent(){
    //read potmeter0 value
    return (double)(MAX_CURRENT*readWiperValue(0))/MAX_POT_VALUE;
}

int CellEmulator::setCurrent(double current){
    //write potmeter0 value
    // int value = MAX_POT_VALUE*current/MAX_CURRENT;
    int value = N_STEPS*current/5-1;
    return writeWiperValue(0, value);
}

int CellEmulator::readWiperValue(int wiperSelect){
    uint8_t data[2];

    selectI2Cbus(cellNumber);
    Wire.beginTransmission(potAddress);
    Wire.write(wiperSelect<<4|0b1100);
    Wire.endTransmission();
    Wire.requestFrom(potAddress, 2);
    while(Wire.available()){
        Wire.readBytes(data, 2);
    }
    return ((data[0]&0x01)<<8)|data[1];
}

int CellEmulator::writeWiperValue(int wiperSelect, uint8_t value){
    selectI2Cbus(cellNumber);
    int size = 2;    
    uint8_t data[size] = {(uint8_t)(wiperSelect<<4),value};
    // tw_master_transmit((potAddress & 0xFE),&data[0],size,0);
    Wire.beginTransmission(potAddress);
    Wire.write(data, size);
    return Wire.endTransmission();
    //                  address     write   data    size repeated start false
}

void selectI2Cbus(int cellNumber){
    if(cellNumber <= I2C_BUS_SLAVE_NUM){
        PORTD |= (1<<I2C_BUS_SELECT_1);
        PORTD &= ~(1<<I2C_BUS_SELECT_2);
    } else{
        PORTD |= (1<<I2C_BUS_SELECT_2);
        PORTD &= ~(1<<I2C_BUS_SELECT_1);
    }
}