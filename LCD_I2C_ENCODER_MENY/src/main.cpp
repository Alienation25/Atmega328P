
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
#include<GyverEncoder.h>


//Подключение єнкодера
#define CLK 2
#define DT 3
#define SW 4// цифровой пин для кнопки


//Экраны для работы с информацией
#define maxD 10
#define minD 1


Encoder enc1(CLK, DT, SW);  // энкодер с кнопкой
LiquidCrystal_I2C lcd(0x27,16,2);//экран Путь I2C и диагональ



enum Language
{
 English,
 Russion,
};

enum Display_panel
{
 DISPLAY_TEMP  = 1,
 DISPLAY_WIND  = 2,
 DISPLAY_LUNIM = 3,

};


volatile unsigned char language = English;//язык
volatile unsigned char numD = 1;//Номер экрана с котором работаем сейчас 






void lcd_input_text(char textE[],char textR[],unsigned char colum,unsigned char row){ //Ввод текста на экран 
  switch (language)
  {

    case English:
     lcd.clear();
     lcd.setCursor(row,colum);
     lcd.print(textE);//команда для ввывода английского языка
    break;
    
    case Russion:
     lcd.clear();
     lcd.setCursor(row,colum);
     lcd.outStr(textR);//команда для ввывода русского языка
    break;
    
   default:
      break;
  }
}




void lcd_input_text(char textE[],char textR[]){
   switch (language)
  {
    case English:
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.print(textE);//команда для ввывода английского языка
    break;
    
    case Russion:
     lcd.clear();
     lcd.setCursor(0,1);
     lcd.outStr(textR);//команда для ввывода русского языка
    break;
  
   default:
      break;
  }
}



void control_Display(unsigned char numD){
  switch (numD)
   {
   case DISPLAY_TEMP://дисплей отвечающий за температуру 
     lcd_input_text("Hello","Привет");  
    
     break;
   case DISPLAY_WIND://дисплей для ветра 
    
    lcd_input_text("WIND: WET","Ветер: ветрений");   
    
     break;
   case DISPLAY_LUNIM://lbcgktq 
     lcd_input_text("DAY: Time","День: время");  
  
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
    
}



void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);
  control_Display(numD);
  enc1.setType(TYPE2);

}






void loop() {

   system_program_encoder();
 
  
}
