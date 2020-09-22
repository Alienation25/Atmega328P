
 //if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  //if (enc1.isLeftH()) Serial.println("Left holded");
  
  //if (enc1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  //if (enc1.isRelease()) Serial.println("Release");     // то же самое, что isClick
  
 // if (enc1.isClick()) Serial.println("Click");         // одиночный клик
 // if (enc1.isSingle()) Serial.println("Single");       // одиночный клик (с таймаутом для двойного)
 // if (enc1.isDouble()) Serial.println("Double");       // двойной клик
  
  
 // if (enc1.isHolded()) Serial.println("Holded");       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки

#include <Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C_OLED.h>
#include <Max44009.h>
#include<SparkFunBME280.h>
#include<GyverEncoder.h>


//Подключение єнкодера
#define CLK 2
#define DT 3
#define SW 4// цифровой пин для кнопки


//Экраны для работы с информацией
#define maxD 10
#define minD 1

enum Language
{
 English,
 Russion,
};


Encoder enc1(CLK, DT, SW);  // энкодер с кнопкой


LiquidCrystal_I2C lcd(0x27,16,2,English);//экран Путь I2C и диагональ
BME280 sensorPressure(0x76);
Max44009 sensorLuxA(0x4A);
Max44009 sensorLuxB(0x4B);



enum Display_panel
{
 DISPLAY_TEMP  = 1,
 DISPLAY_WIND  = 2,
 DISPLAY_LUNIM = 3,

};

volatile unsigned char numD = 1;//Номер экрана с котором работаем сейчас 
volatile float LuxA = 0 ;
volatile float LuxB = 0 ;
volatile double Pressure=0;











void control_Display(unsigned char numD){
  switch (numD)
   {
   case DISPLAY_TEMP://дисплей отвечающий за температуру 
     lcd.clear();
     
    
     break;
   case DISPLAY_WIND://дисплей для ветра 
     lcd.clear();
   
  
     break;
   case DISPLAY_LUNIM://дисплей для света  
     lcd.clear();
    
  
    break;
   default:
     break;
   }
}














void isrCLK() {
  enc1.tick();  // отработка в прерывании
  if (enc1.isLeft()){
          numD--;
         if(!(minD <= (int)numD)){
           numD=minD;
         }
         
       
  }
}



void isrDT() {
  enc1.tick();  // отработка в прерывании
  if (enc1.isRight()) {
           numD++;
         if(!((int)numD < maxD)){
           numD=maxD;
         }
          
  } 
}







void setup() {
 Serial.begin(9600);
 enc1.setType(TYPE2);
 
 lcd.backlight();
 lcd.init();
 lcd.clear();
 lcd.setCursor(0,0);

 attachInterrupt(0, isrCLK, CHANGE);    // прерывание на 2 пине! CLK у энка
 attachInterrupt(1, isrDT, CHANGE);    // прерывание на 3 пине! DT у энка
  
  if(sensorPressure.beginI2C() == false) Serial.println("Sensor A connect failed");


}


void loop() {
   enc1.tick();
   control_Display(numD);
   LuxA=sensorLuxA.getLux();
   LuxB=sensorLuxB.getLux();
   Pressure=sensorPressure.readFloatPressure();  
   delay(800);


}

