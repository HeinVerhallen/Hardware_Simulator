#include "CellEmulator.h"
#include "twi_master.h"

CellEmulator::CellEmulator(uint8_t address, uint8_t number){
    potAddress = address;
    cellNumber = number;
    tw_init(TW_FREQ_250K, 0);
}

CellEmulator::~CellEmulator(){

}

double CellEmulator::getVoltage(){
    //read potmeter1 value
    return (double)V_REF*N_STEPS/(N_STEPS-readWiperValue(1));
}

void CellEmulator::setVoltage(double voltage){
    uint8_t value = N_STEPS-(V_REF*N_STEPS/voltage);
    writeWiperValue(1, value);
    //write potmeter1 value
}

double CellEmulator::getCurrent(){
    //read potmeter0 value
    return (double)(MAX_CURRENT*readWiperValue(0))/MAX_POT_VALUE;
}

void CellEmulator::setCurrent(double current){
    //write potmeter0 value
    //potvalue=maxvalue*current/maxcurrent
    uint8_t value = MAX_POT_VALUE*current/MAX_CURRENT;
    writeWiperValue(0, value);
}

uint8_t CellEmulator::readWiperValue(uint8_t wiperSelect){
    selectI2Cbus(cellNumber);
    uint8_t data[2] = {(wiperSelect<<4)&(0b11<<2)};
    //                          register addres     read mode + 2reserved

    //                  address     write   data   size repeated start true
    tw_master_transmit((potAddress & 0xFE),&data[0],1,1);
    tw_master_receive((potAddress | 0x1),&data[0], 2);
    //                  address     read    data    size

    return data[1];
}

void CellEmulator::writeWiperValue(uint8_t wiperSelect, uint8_t value){
    selectI2Cbus(cellNumber);
    uint8_t size = 2;    
    uint8_t data[size] = {(wiperSelect<<4),value};
    tw_master_transmit((potAddress & 0xFE),&data[0],size,0);
    //                  address     write   data    size repeated start false
}

void selectI2Cbus(int cellNumber){
    if(cellNumber > 1){
        return;
    }
    PORTD |= (cellNumber<<I2C_BUS_SELECT_1) | (!cellNumber<<I2C_BUS_SELECT_2);
}