#include <Arduino.h>
// Пример использования меню для построения диалога



#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#include <Max44009.h>
#include <SparkFunBME280.h>

LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);
Max44009 LuxA(0x4A);
Max44009 LuxB(0x4B);

// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4

// Объявим перечисление, используемое в качестве ключа пунктов меню

enum {mkRoot,mkBack, mkLux, mkHumidity, mkC02, mkPressure, mktemperature,mklanguage};

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
sMenuItem menu[] = {
    {mkBack, mkLux, "Датчик света"},
    {mkBack, mkHumidity, "Датчик влажности"},
    {mkBack, mkC02, "Датчик С02"},
    {mkBack, mkPressure, "Датчик давления"},
    {mkBack, mktemperature, "Датчик температуры"},
    {mkBack, mklanguage, "Язык"},
    {mkRoot, mkBack, "Exit menu"}
};

uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

void setup() {
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
}


int i=1;
void loop() {


  // Показываем меню
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 0);
  // И выполняем действия в соответствии с выбранным пунктом
  if (selectedMenuItem == mkLux){
     do
     {
    delay(100);
    i++;
    lcd.printfAt(0, 0, "LuxA=%f", LuxA.getLux());
    lcd.printfAt(0, 1, "LuxB=%d", i);
     } while (lcd.getEncoderState() == eNone);
    
    }
  else if (selectedMenuItem == mkHumidity)
    lcd.print("Mode selected");
  else if (selectedMenuItem == mkC02)
    lcd.print("Speed selected");
  else if (selectedMenuItem == mkPressure)
    lcd.print("Print log selected");
  else if (selectedMenuItem == mktemperature)
    lcd.print("Selftest selected");
  else if (selectedMenuItem == mkBack)
    lcd.print("Exit selected");
  while (lcd.getEncoderState() == eNone);
}