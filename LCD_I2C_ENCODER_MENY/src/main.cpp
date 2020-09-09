#include <Arduino.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
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

byte bukva_B[8]   = {B11110,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Б"
byte bukva_G[8]   = {B11111,B10001,B10000,B10000,B10000,B10000,B10000,B00000,}; // Буква "Г"
byte bukva_D[8]   = {B01111,B00101,B00101,B01001,B10001,B11111,B10001,B00000,}; // Буква "Д"
byte bukva_ZH[8]  = {B10101,B10101,B10101,B11111,B10101,B10101,B10101,B00000,}; // Буква "Ж"
byte bukva_Z[8]   = {B01110,B10001,B00001,B00010,B00001,B10001,B01110,B00000,}; // Буква "З"
byte bukva_I[8]   = {B10001,B10011,B10011,B10101,B11001,B11001,B10001,B00000,}; // Буква "И"
byte bukva_IY[8]  = {B01110,B00000,B10001,B10011,B10101,B11001,B10001,B00000,}; // Буква "Й"
byte bukva_L[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,}; // Буква "Л"
byte bukva_P[8]   = {B11111,B10001,B10001,B10001,B10001,B10001,B10001,B00000,}; // Буква "П"
byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // Буква "У"
byte bukva_F[8]   = {B00100,B11111,B10101,B10101,B11111,B00100,B00100,B00000,}; // Буква "Ф"
byte bukva_TS[8]  = {B10010,B10010,B10010,B10010,B10010,B10010,B11111,B00001,}; // Буква "Ц"
byte bukva_CH[8]  = {B10001,B10001,B10001,B01111,B00001,B00001,B00001,B00000,}; // Буква "Ч"
byte bukva_Sh[8]  = {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00000,}; // Буква "Ш"
byte bukva_Shch[8]= {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00001,}; // Буква "Щ"
byte bukva_Mz[8]  = {B10000,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Ь"
byte bukva_IYI[8] = {B10001,B10001,B10001,B11001,B10101,B10101,B11001,B00000,}; // Буква "Ы"
byte bukva_Yu[8]  = {B10010,B10101,B10101,B11101,B10101,B10101,B10010,B00000,}; // Буква "Ю"
byte bukva_Ya[8]  = {B01111,B10001,B10001,B01111,B00101,B01001,B10001,B00000,}; // Буква "Я"


volatile unsigned char language = 1;//1-English 2-Russion 
volatile unsigned char numD = 1;//Номер экрана с котором работаем сейчас 


void lcd_input_text(String info,unsigned char colum,unsigned char row){ //Ввод текста на экран 
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

void language_setting(int lang,String english,String russion)
{
 


}




void control_Display(unsigned char numD){
  switch (numD)
   {
   case DISPLAY_TEMP:

    
     break;
   case DISPLAY_WIND:
    lcd_input_text("WIND: WET");   
     break;
   case DISPLAY_LUNIM:
     lcd_input_text("DAY: Time");
  //if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  //if (enc1.isLeftH()) Serial.println("Left holded");
  
  //if (enc1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  //if (enc1.isRelease()) Serial.println("Release");     // то же самое, что isClick
  
 // if (enc1.isClick()) Serial.println("Click");         // одиночный клик
 // if (enc1.isSingle()) Serial.println("Single");       // одиночный клик (с таймаутом для двойного)
 // if (enc1.isDouble()) Serial.println("Double");       // двойной клик
  
  
 // if (enc1.isHolded()) Serial.println("Holded");       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки
  
  
    break;
   default:
     break;
   }
}





void system_program_encoder(){
    enc1.tick();
    if(enc1.isTurn()){
      if (enc1.isRight())      // 
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
 
  
}
