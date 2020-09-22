#include <Arduino.h>

#include <Wire.h>

#include <SparkFunBME280.h>

BME280 mySensorA(0x76); 


void setup()
{
  Serial.begin(9600);
  Serial.println("Example showing alternate I2C addresses");

  

 //The default for the SparkFun Environmental Combo board is 0x77 (jumper open).
  //If you close the jumper it is 0x76
  //The I2C address must be set before .begin() otherwise the cal values will fail to load.

  if(mySensorA.beginI2C() == false) Serial.println("Sensor A connect failed");


}

void loop()
{
  Serial.print("HumidityA: ");
  Serial.print(mySensorA.readFloatHumidity(), 0);

  Serial.print(" PressureA: ");
  Serial.print(mySensorA.readFloatPressure(), 0);

  Serial.print(" TempA: ");
  Serial.print(mySensorA.readTempC(), 2);
  //Serial.print(mySensorA.readTempF(), 2);


  Serial.println();

  delay(50);
}

