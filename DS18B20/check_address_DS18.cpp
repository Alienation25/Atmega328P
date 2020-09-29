

#include <Arduino.h>
// библиотека для работы с протоколом 1-Wire
#include <OneWire.h>
// библиотека для работы с датчиком DS18B20 
// сигнальный провод датчика
#define ONE_WIRE_BUS 10
 
// Based on the OneWire library example
 
OneWire ds(ONE_WIRE_BUS);  //data wire connected to GPIO15
byte addr [2][8];



void checkAddress( byte addr [2][8]){
   for(int i=0;i<2;i++){
      if (!ds.search(addr[i])) {
        Serial.println(" No more addresses.");
        ds.reset_search();
        delay(250);
      return;
       }
      
      if (OneWire::crc8(addr[i], 7) != addr[i][7]) {
        Serial.println("CRC is not valid!");
        return;
      }
   }
 ds.reset_search();
}


void setup(void) {
  Serial.begin(9600);
  checkAddress(addr);
 // checkAddress(addr);
}
 
void loop(void) {
  for(int i=0;i<2;i++){
   for(int j=0;j<8;j++){
      Serial.write(' ');
      Serial.print(addr[i][j], HEX);
   }
  }
   delay(10000);
   
  }