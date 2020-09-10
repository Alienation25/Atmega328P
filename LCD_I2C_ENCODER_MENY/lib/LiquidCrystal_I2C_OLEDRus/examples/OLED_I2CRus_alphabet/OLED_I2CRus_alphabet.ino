#include <Wire.h> 
#include <LiquidCrystal_I2C_OLED.h>
LiquidCrystal_I2C OLED1(0x3F,16,2);  // Устанавливаем дисплей
void setup()
{ 
  OLED1.init();                     
  OLED1.clear();
  OLED1.print("Proba"); //вывод английского
  OLED1.setCursor(7, 0); //середина верхней строки
  OLED1.outStr("Проба"); //вывод русского
  OLED1.setCursor(7, 1);//середина нижней строки
  OLED1.print("-22,3\337C"); //"-22,3°C"
  delay(1000);
}
void loop()
{//для дисплея 16х2 или 20х2!!!!
  OLED1.setCursor(0, 0); //начало верхней строки
  OLED1.outStr("АБВГДЕЖЗИЙКЛМНОП");
  OLED1.setCursor(0, 1);//начало нижней строки
  OLED1.outStr("абвгдежзийклмноп");
  delay(2000);
  OLED1.setCursor(0, 0); //начало верхней строки
  OLED1.outStr("РСТУФХЦЧШЩЪЫЬЭЮЯ");
  OLED1.setCursor(0, 1);//начало нижней строки
  OLED1.outStr("рстуфхцчшщъыьэюя");
  delay(3000);
}
