#include <Arduino.h>
// библиотека для работы с протоколом 1-Wire
#include <OneWire.h>
// библиотека для работы с датчиком DS18B20
#include <DallasTemperature.h>
 
// сигнальный провод датчика
#define ONE_WIRE_BUS 10
 
// Based on the OneWire library example
 
OneWire ds(ONE_WIRE_BUS);  //data wire connected to GPIO15
 
void setup(void) {
  Serial.begin(9600);
}
 
void loop(void) {
  byte i;
  byte addr[8];
 
  if (!ds.search(addr)) {
    Serial.println(" No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  Serial.print(" ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(addr[i], HEX);
  }
}