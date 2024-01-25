#include <Arduino.h>
#include "CellEmulator.h"

// initialize the cell emulators with their addresses and numbers
  CellEmulator pot1(0b00101000, 0);
  // CellEmulator pot2(0b00101001, 0);
  // CellEmulator pot3(0b00101010, 1);

  char errorMessage[7][16] = {{"Succes"},{ "Nack on data"}, {"Nack on address"}, {"Data too long"}, {"Other error"}, {"Time out"}, {"Data error"}};

  void testFunction(double val){
  Serial.print("wiper setting should be: ");
  Serial.println(val);
  // Serial.println(pot1.setVoltage((double)voltage),BIN);
  // Serial.println(errorMessage[pot1.setVoltage(val)]); //select wiper 0
  // Serial.println(pot1.getVoltage());
  Serial.println(errorMessage[pot1.setCurrent(val)]); //select wiper 0
  // Serial.println(pot1.getCurrent());
}

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

      if(buffer[idx] == 0x72){ //if the data is a 'r'
        double testValue = (buffer[idx-3]-48)+(double)(buffer[idx-2]-48)/10+(double)(buffer[idx-1]-48)/100; //convert the data to an int
        Serial.print("testValue: ");
        Serial.println(testValue);
        testFunction(testValue); //call the test function
      }
      idx++;
    }
  } 
  return 0;
}