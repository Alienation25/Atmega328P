
#include <Arduino.h>
#include <OneWire.h>

OneWire ds(10); // Объект OneWire

int temperature = 0; // Глобальная переменная для хранения значение температуры с датчика DS18B20

long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок

void setup(){
  Serial.begin(9600);
}
 byte ds18A[8] = {0x28, 0x38, 0xE,0x5C, 0x1A, 0x19, 0x1,0x1A };
 byte ds18B[8] = {0x28, 0xAD, 0x62 ,0x41,0x1A, 0x19,0x1,0xD5 };




int detectTemperature(byte addres[8]){  //Функция для работы с датчиками

  byte data[2];
  ds.reset();
  ds.select(addres);
  ds.write(0x44);

  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    lastUpdateTime = millis();
    ds.reset();
    ds.select(addres);
    ds.write(0xBE);
    data[0] = ds.read();
    data[1] = ds.read();

    // Формируем значение
    temperature = (data[1] << 8) + data[0]; temperature = temperature >> 4;
  }
}


void loop(){
  detectTemperature(ds18A); // Определяем температуру от датчика DS18b20
  Serial.println(temperature);
  Serial.print(" A");

  detectTemperature(ds18B); // Определяем температуру от датчика DS18b20
  Serial.println(temperature); // Выводим полученное значение температуры
  Serial.print(" B");
 
 
  // Т.к. переменная temperature имеет тип int, дробная часть будет просто отбрасываться
}






