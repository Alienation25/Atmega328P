
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
float LuxA = 0 ;
float LuxB = 0 ;
float Pressure=0;
char buffer[8];
String display_text;

void control_Display(unsigned char numD){
  switch (numD)
   {
   case DISPLAY_TEMP://дисплей отвечающий за температуру 
     lcd.clear();
     
     display_text="";  
     break;
   case DISPLAY_WIND://дисплей для ветра 
     lcd.clear();
      //display_text="Pres ";
      //display_text+=dtostrf(Pressure,3,1,buffer);
      lcd.print(sensorPressure.readFloatPressure());    
    
     display_text="";  
  


     break;
   case DISPLAY_LUNIM://lbcgktq 
     lcd.clear();
     display_text="luxA ";
     display_text+=dtostrf(LuxA,3,1,buffer);
     display_text+="luxB ";
     display_text+=dtostrf(LuxB,3,1,buffer);
      
      lcd.print(display_text.substring(0,9));  
      lcd.setCursor(0,1);
      lcd.print(display_text.substring(9));
      display_text="";  
  
    break;
   default:
     break;
   }
}


void system_program_encoder(){//функция для отлова движения энкодера   
   
    enc1.tick(); 
    if(enc1.isTurn())//если произошло движения энкодера в любую сторону
    {
    
      if (enc1.isRight())//  если произошло движение энкодера в право    
       {
         numD++;
         if(!((int)numD < maxD)){
           numD=maxD;
         }
         control_Display(numD);
       }


      if (enc1.isLeft()) //  если произошло движение энкодера в лево 
       {
         numD--;
         if(!(minD <= (int)numD)){
           numD=minD;
         }
           control_Display(numD);
       }
    }
     if(enc1.isPress())
     {
       lcd.chenglanguage(1);


     }
    
}



void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  control_Display(numD);
  enc1.setType(TYPE2);
  
}






void loop() {
   system_program_encoder();

   LuxA=sensorLuxA.getLux();
   LuxB=sensorLuxB.getLux();
//   Pressure=sensorPressure.readFloatPressure();
  
   Serial.print(" PressureA: ");
  Serial.print(sensorPressure.readFloatPressure(), 0);


}
