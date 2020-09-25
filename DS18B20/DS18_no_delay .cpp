#include <Arduino.h>
#include <OneWire.h>

OneWire ds(8); // Объект OneWire

int temperature = 0; // Глобальная переменная для хранения значение температуры с датчика DS18B20

long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок

void setup(){
  Serial.begin(9600);
}

void loop(){
  detectTemperature(); // Определяем температуру от датчика DS18b20
  Serial.println(temperature); // Выводим полученное значение температуры
  // Т.к. переменная temperature имеет тип int, дробная часть будет просто отбрасываться
}

int detectTemperature(){

  byte data[2];
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);

  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    lastUpdateTime = millis();
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);
    data[0] = ds.read();
    data[1] = ds.read();

    // Формируем значение
    temperature = (data[1] << 8) + data[0]; temperature = temperature >> 4;
  }
}