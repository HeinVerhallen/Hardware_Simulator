#include <Arduino.h>
#include "CellEmulator.h"

// #define DEBUG_MODE 1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int state = 0;
CellEmulator pot1(0b00101000, 1);
CellEmulator pot2(0b00101001, 2);
CellEmulator pot3(0b00101010, 8);

void testFunction(uint8_t voltage){
  Serial.print("voltage should be: ");
  Serial.println(voltage);
  Serial.println(voltage,BIN);
  Serial.println(pot1.setVoltage((double)voltage),BIN);
  // Serial.println(pot1.setCurrent(1));
  // Serial.print("voltage reading: ");
  int test = pot1.getVoltage();
  Serial.println(test,BIN);
  // Serial.println(pot2.setVoltage((double)voltage-1));
  // Serial.println(pot2.setCurrent((double)voltage-1));
  // Serial.println(pot2.setVoltage((double)voltage-1));
  // Serial.println(pot3.setCurrent((double)voltage-2));
}

void loop() {
  // char receiveVal;   
  int size = 300;
  char buffer[size];
  int idx = 0;

  // for(int i = 0; i < 6; i++){
  //   testFunction(i);
  //   delay(1000);
  // }
  
  while(Serial.available() > 0)
    {
      if (idx > size){
        break;
      } else{
        buffer[idx] = Serial.read();
      }

      if(buffer[idx] == 0x72){
        testFunction(buffer[idx-1]-48);
      }
      idx++;
    }  


    delay(50);   
}
