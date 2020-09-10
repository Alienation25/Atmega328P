#include <Wire.h> 
#include <LiquidCrystal_I2C_OLED.h>
LiquidCrystal_I2C OLED1(0x3F,16,2);  // Устанавливаем дисплей
void setup()
{ 
  OLED1.init();                     
  OLED1.clear();
  OLED1.print("Proba"); //вывод английского
  OLED1.setCursor(7, 0); //середина верхней строки
  OLED1.outStr("Проба\337C");//вывод вперемешку, символ '°C' - английский
  OLED1.setCursor(7, 1);//середина нижней строки
  OLED1.print("-22,3\337C"); //"-22,3°C"
}
void loop()
{
  // Устанавливаем курсор на вторую строку и нулевой символ.
  OLED1.setCursor(0, 1); //начало нижней строки
  // Выводим на экран количество секунд с момента запуска ардуины
  OLED1.print(millis()/1000);
}
