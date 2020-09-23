#include <Arduino.h>
// Пример использования меню для построения диалога
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#include <Max44009.h>
#include <SparkFunBME280.h>





BME280 Press(0x76);//рудимент
Max44009 LuxA(0x4A);//рудимент
Max44009 LuxB(0x4B);//рудимент
LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);


// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4
///

//пины управления 
#define accuracy 2 //значения после точки(точность) 
#define refresh_display 100 //
uint8_t language=1; //язык интерфейса


// Объявим перечисление, используемое в качестве ключа пунктов меню
enum {Russion,English};//потдержка языков
enum {mkRoot,mkBack, mkLux, mkHumidity, mkC02, mkPressure, mktemperature,mklanguage};

//printWL("Датчик света","Sensor light").

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}



const char* printWL(String Rus,String Eng){            //ПРОВЕРИТЬ ФУНКЦИЮ НА ПАМЯТЬ
 unsigned char* buf;
 buf = new unsigned char[20];
 switch (language)
 {
 case Russion: 
   Rus.getBytes(buf, 20, 0);
 break;

case English:
   Eng.getBytes(buf, 20, 0);
 break;

default:
   Serial.print("Error: var language error data"); 
   break;
 }

  return (const char *)buf ;
}




//printWL("Датчик света","Light sepnsor")
sMenuItem menu[] = {
    {mkBack, mkLux,"Датчик света"},
    {mkBack, mkHumidity,"Датчик влажности"},
    {mkBack, mkC02, "Датчик С02"},
    {mkBack, mkPressure,"Датчик давления"},
    {mkBack, mktemperature,"Датчик температуры"},
    {mkBack, mklanguage,"Язык"},
    {mkRoot, mkBack, "Exit menu"}
};

uint8_t menuLen = sizeof(menu) / sizeof(sMenuItem);

//Структура для хранения значений датчиков 






void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  if (Press.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
  }
}



void sensor_display(String name,float number_sensor,String unit,char r,char c){//функция для вывлда показаний датчика
    String buffer= String(number_sensor,accuracy);
    lcd.printfAt(r,c,"%s=%s%s",name.c_str(),buffer.c_str(),unit.c_str());
   
}





void loop() {
  // Показываем меню
  uint8_t selectedMenuItem = lcd.showMenu(menu, menuLen, 0);
  // И выполняем действия в соответствии с выбранным пунктом
  
  
  
  
  
  if (selectedMenuItem == mkLux){
       do
       {
       Serial.print(lcd.getEncoderState());
       sensor_display("Свет",LuxA.getLux(),"hpa",0,0) ; 
       sensor_display("Свет",LuxB.getLux(),"hpa",0,1) ; 
       delay(refresh_display);

       } while (1);
    }
 
  else if (selectedMenuItem == mkHumidity)
    lcd.print("Fals");
  
  else if (selectedMenuItem == mkC02)
    lcd.print("Fals");

  else if (selectedMenuItem == mkPressure){
     do
    { 
      sensor_display("Давление",Press.readFloatHumidity(),"hpa",0,0) ; 
      delay(refresh_display);
       
    } while (lcd.getEncoderState() == eNone);
    
  }
    
   
  else if (selectedMenuItem == mktemperature)
    lcd.print("Selftest selected");
  else if (selectedMenuItem == mkBack)
    lcd.print("Exit selected");

  while (lcd.getEncoderState() == eNone);

}




