#include <Arduino.h>
#include "CellEmulator.h"

// #define DEBUG_MODE 1

  char errorMessage[8][19] = {{"Succes"}, {"Data too long"} ,{"Nack on address"}, {"Nack on data"}, {"Other error"}, {"Time out"}, {"Data error"}, {"value out of range"}}; //error messages

  CellEmulator pot1(0x28, 0x2C, 0);//current+temperature pot address, voltage pot address, cell emulator number

int main(void){
  init();
  Serial.begin(9600);

  while(1){
    int size = 300;
    char buffer[size];
    static int idx = 0;

    //check if data is available on the serial port
    while(Serial.available() > 0){
      if (idx > size) //if the buffer is full
        break;
      else //if the buffer is not full
        buffer[idx] = Serial.read(); //read the data from the serial port

      if((buffer[idx] == 'i') | (buffer[idx] == 'I')){ //run the current function
        double testValue = (buffer[idx-3]-48)*10+(double)(buffer[idx-2]-48)+(double)(buffer[idx-1]-48)/10; //convert the data to an int
        Serial.println(testValue);
        #ifdef DEBUG_MODE
          Serial.println(pot1.setCurrent(testValue));
        #else
          Serial.println(errorMessage[pot1.setCurrent(testValue)]);
        #endif
      }

      if((buffer[idx] == 'v') | (buffer[idx] == 'V')){
        double testValue = (buffer[idx-3]-48)+(double)(buffer[idx-2]-48)/10+(double)(buffer[idx-1]-48)/100; //convert the data to an int
        Serial.println(testValue);
        #ifdef DEBUG_MODE
          Serial.println(pot1.setVoltage(testValue));
        #else
          Serial.println(errorMessage[pot1.setVoltage(testValue)]);
        #endif
      }

      if((buffer[idx] == 't') | (buffer[idx] == 'T')){
        double testValue = (buffer[idx-3]-48)*10+(double)(buffer[idx-2]-48)+(double)(buffer[idx-1]-48)/10; //convert the data to an int
        Serial.println(testValue);
        #ifdef DEBUG_MODE
          Serial.println(pot1.setTemperature(testValue));
        #else
          Serial.println(errorMessage[pot1.setTemperature(testValue)]);
        #endif
      }
      idx++;
    }
  } 
  return 0;
}