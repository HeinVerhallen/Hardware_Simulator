#include <Arduino.h>
#include "CellEmulator.h"

// debug mode selects a different algorithm to test the potentiometers directly (comment out to disable)
// #define DEBUG_MODE 1

// initialize the cell emulators with their addresses and numbers
  CellEmulator pot1(0b00101000, 1);
  CellEmulator pot2(0b00101001, 2);
  CellEmulator pot3(0b00101010, 8);

int main(void){
  Serial.begin(9600);

  while(1){
    int size = 300;
    char buffer[size];
    int idx = 0;

    //check if data is available on the serial port
    while(Serial.available() > 0)
      {
        if (idx > size){ //if the buffer is full
          break;
        } else{ //if the buffer is not full
          buffer[idx] = Serial.read(); //read the data from the serial port
        }

        if(buffer[idx] == 0x72){ //if the data is a 'r'
          testFunction(buffer[idx-1]-48); //call the test function
        }
        idx++;
      }
    } 
  return 0;
}

void testFunction(uint8_t voltage){
  Serial.print("voltage should be: ");
  Serial.println(voltage);
  // Serial.println(pot1.setVoltage((double)voltage),BIN);
  Serial.println(pot1.setCurrent(voltage));
}