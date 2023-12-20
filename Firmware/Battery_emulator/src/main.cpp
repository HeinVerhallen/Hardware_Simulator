#include <Arduino.h>
#include "CellEmulator.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

int state = 0;
CellEmulator pot1(0b00101000, 1);
CellEmulator pot2(0b00101001, 2);
CellEmulator pot3(0b00101010, 8);

void testFunction(int voltage){
  Serial.print("voltage should be: ");
  Serial.println(voltage);
  Serial.println(pot1.setVoltage((double)voltage));
  Serial.println(pot1.setCurrent((double)voltage-1));
  Serial.print("voltage reading: ");
  Serial.println(pot1.getVoltage());
  Serial.println(pot2.setVoltage((double)voltage-1));
  Serial.println(pot2.setCurrent((double)voltage-1));
  Serial.println(pot2.setVoltage((double)voltage-1));
  Serial.println(pot3.setCurrent((double)voltage-2));
}

void loop() {
  // char receiveVal;   
  int size = 300;
  char buffer[size];
  int idx = 0;
  
  while(Serial.available() > 0)
    {
      if (idx > size)
      {
        break;
      }
      else
      {
        buffer[idx] = Serial.read();
      }

      Serial.write(buffer[idx]);
      if(buffer[idx] == 0x72){
        testFunction(buffer[idx-1]-48);
      }
      idx = (idx+1)%size;
    }  


    delay(50);   
}
