#include <Arduino.h>
#include "CellEmulator.h"

// #define DEBUG_MODE 1

  char errorMessage[8][19] = {{"Succes"}, {"Data too long"} ,{"Nack on address"}, {"Nack on data"}, {"Other error"}, {"Time out"}, {"Data error"}, {"value out of range"}}; //error messages

  CellEmulator pot1(0x28, 0x2C, 0);//current+temperature pot address, voltage pot address, cell emulator number

int main(void){
  //setup
  init();
  Serial.begin(9600);

  while(1){
    int size = 300;
    char buffer[size];
    static int idx = 0;

    //check if data is available on the serial port
    while(Serial.available() > 0){ //read data from serial port and store it in the buffer
      if (idx > size) //if the buffer is full
        break;
      else
        buffer[idx] = Serial.read();


      if((buffer[idx] == 'i') | (buffer[idx] == 'I')){ //run the current function
        //convert convert the data to usefull testValue
        double testValue = (buffer[idx-3]-48)*10+(double)(buffer[idx-2]-48)+(double)(buffer[idx-1]-48)/10;
        Serial.println(testValue);
        #ifdef DEBUG_MODE
          //in debug mode, the calculated value is printed to the serial monitor
          Serial.println(pot1.setCurrent(testValue));
        #else
          //in normal mode, the error message is printed to the serial monitor
          Serial.println(errorMessage[pot1.setCurrent(testValue)]);
        #endif
      }

      if((buffer[idx] == 'v') | (buffer[idx] == 'V')){ //run the voltage function
        //convert convert the data to usefull testValue
        double testValue = (buffer[idx-3]-48)+(double)(buffer[idx-2]-48)/10+(double)(buffer[idx-1]-48)/100;
        Serial.println(testValue);
        #ifdef DEBUG_MODE
          //in debug mode, the calculated value is printed to the serial monitor
          Serial.println(pot1.setVoltage(testValue));
        #else
          //in normal mode, the error message is printed to the serial monitor
          Serial.println(errorMessage[pot1.setVoltage(testValue)]);
        #endif
      }

      if((buffer[idx] == 't') | (buffer[idx] == 'T')){ //run the temperature function
        //convert convert the data to usefull testValue
        double testValue = (buffer[idx-3]-48)*10+(double)(buffer[idx-2]-48)+(double)(buffer[idx-1]-48)/10;
        Serial.println(testValue);
        #ifdef DEBUG_MODE
          //in debug mode, the calculated value is printed to the serial monitor
          Serial.println(pot1.setTemperature(testValue));
        #else
          //in normal mode, the error message is printed to the serial monitor
          Serial.println(errorMessage[pot1.setTemperature(testValue)]);
        #endif
      }
      idx++;
    }
  } 
  return 0;
}