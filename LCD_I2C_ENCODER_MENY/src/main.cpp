#include <Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<GyverEncoder.h>

#define CLK 2
#define DT 3
#define SW 4
#define maxD 10//Последний экран
#define minD 1//Первый экран
//Encoder enc1(CLK, DT);      // для работы без кнопки
Encoder enc1(CLK, DT, SW);  // для работы c кнопкой
LiquidCrystal_I2C lcd(0x27,16,2);
//Encoder enc1(CLK, DT, SW, TYPE2);  // для работы c кнопкой и сразу выбираем тип
//Encoder enc1(CLK, DT, ENC_NO_BUTTON, TYPE2);  // для работы без кнопки и сразу выбираем тип
// Варианты инициализации:
// Encoder enc;									// не привязан к пину
// Encoder enc(пин CLK, пин DT);				// энкодер без кнопки (ускоренный опрос)
// Encoder enc(пин CLK, пин DT, пин SW);		// энкодер с кнопкой
// Encoder enc(пин CLK, пин DT, пин SW, тип);	// энкодер с кнопкой и указанием типа
// Encoder enc(пин CLK, пин DT, ENC_NO_BUTTON, тип);	// энкодер без кнопкой и с указанием типа

volatile unsigned char numD = 1;
//Display max or min


void lcd_input_text(String info,unsigned char colum,unsigned char row){
   lcd.clear();
   lcd.setCursor(row,colum);
   lcd.print(info);
   
}
void lcd_input_text(String info){
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print(info);
   
}
void lcd_input_text(unsigned char info){
   lcd.clear();
   lcd.setCursor(0,1);
   lcd.print(info);
   
}





enum Display_panel
{
 DISPLAY_TEMP  = 1,
 DISPLAY_WIND  = 2,
 DISPLAY_LUNIM = 3,

};


void control_Display(unsigned char numD){
  switch (numD)
   {
   case DISPLAY_TEMP:
    lcd_input_text("Temp: COOLD");
     break;
   case DISPLAY_WIND:
    lcd_input_text("WIND: WET");
     break;
   case DISPLAY_LUNIM:
     lcd_input_text("DAY: Time");
    break;
   default:
     break;
   }
}





void system_program_encoder(){
    enc1.tick();
    if(enc1.isTurn()){
      if (enc1.isRight())      // если был поворот
       {
         numD++;
         if(!((int)numD < maxD)){
           numD=maxD;
         }
         control_Display(numD);
       }

      if (enc1.isLeft()) 
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


 
  //if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  //if (enc1.isLeftH()) Serial.println("Left holded");
  
  //if (enc1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  //if (enc1.isRelease()) Serial.println("Release");     // то же самое, что isClick
  
 // if (enc1.isClick()) Serial.println("Click");         // одиночный клик
 // if (enc1.isSingle()) Serial.println("Single");       // одиночный клик (с таймаутом для двойного)
 // if (enc1.isDouble()) Serial.println("Double");       // двойной клик
  
  
 // if (enc1.isHolded()) Serial.println("Holded");       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки
}
