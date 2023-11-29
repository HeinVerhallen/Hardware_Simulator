#include <Arduino.h>
#include "CellEmulator.h"

int LedPin = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  CellEmulator cell1 = new CellEmulator(adress, 1);

  pinMode(LedPin, OUTPUT);
}

void loop() {
  // char receiveVal;   
  int size = 300;
  char buffer[size];
  int idx = 0;

  for (int i = 0; i < size; i++)
  {
    buffer[i] = 0;
  }
  
  while(Serial.available() > 0)
    {
      if (idx > size)
      {
        Serial.read();
      }
      else
      {
        buffer[idx++] = Serial.read();
      }

      //  receiveVal = Serial.read();

      //  if (receiveVal == 'H')
      //  {
      //   digitalWrite(LedPin, HIGH);
      //  }
      //  if (receiveVal == 'L')
      //  {
      //   digitalWrite(LedPin, LOW);
      //  }
    }  

    Serial.write(buffer);

    delay(50);   
}
