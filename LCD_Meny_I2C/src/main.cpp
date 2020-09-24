#include <Arduino.h>
// Пример использования меню для построения диалога
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#include <Max44009.h>
#include <SparkFunBME280.h>
#include <TroykaMQ.h>




#define PIN_MQ135  A7

BME280 BME280A(0x76);//рудимент
Max44009 LuxA(0x4A);//рудимент
Max44009 LuxB(0x4B);//рудимент
MQ135 mq135(PIN_MQ135);



LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);


// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4
///

//пины управления 

//#define accuracy 2 //значения после точки(точность) 
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


String cheng_language(String Rus ,String Eng){

   if (language=Russion)
   {
      return Rus;
   
   }
    if (language=English)
   {
     return Eng;
   }
   
}





//printWL("Датчик света","Light sepnsor")
sMenuItem menuRussion[] = {
    {mkBack, mkLux,"Датчик света"},
    {mkBack, mkHumidity,"Датчик влажности"},
    {mkBack, mkC02, "Датчик С02"},
    {mkBack, mkPressure,"Датчик давления"},
    {mkBack, mktemperature,"Датчик температуры"},
    {mkBack, mklanguage,"Язык"},
    {mkRoot, mkBack, "Exit menu"}
};






uint8_t menuLen = sizeof(menuRussion) / sizeof(sMenuItem);

//Структура для хранения значений датчиков 






void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  mq135.calibrate();
  
  Serial.print("Ro = ");
  Serial.println(mq135.getRo());
  
  if (BME280A.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
  }
}



void sensor_display(String name,float number_sensor,String unit,char r,char c,char accuracy){//функция для вывлда показаний датчика
    int num;
    String buffer= String(number_sensor,accuracy);
    num = buffer.length()+name.length()+unit.length();        
    lcd.printfAt(r,c,"%s=%s%s",name.c_str(),buffer.c_str(),unit.c_str());
    lcd.printfAt(r+=num,c,"      ");

   
}



float val = 0; 

void loop() {
  // Показываем меню
  
  uint8_t selectedMenuItem = lcd.showMenu(menuRussion, menuLen, 0);
 
  
  // И выполняем действия в соответствии с выбранным пунктом
  
  if (selectedMenuItem == mkLux){  //два датчика GY-49
      
       do{
       sensor_display("Свет",LuxA.getLux(),"лм",0,0,1) ; 
       sensor_display("Свет",LuxB.getLux(),"лм",0,1,1) ; 
       delay(refresh_display);
       } while (lcd.getEncoderState() == eNone);
       
    }
 
  else if (selectedMenuItem == mkHumidity) //датчик  BME-280(влажности)
       do{
       sensor_display("Влага",BME280A.readFloatHumidity(),"%",0,0,1) ; 
       delay(refresh_display);
     } while (lcd.getEncoderState() == eNone);
       
  
  else if (selectedMenuItem == mkC02) //10 
        do{  
        sensor_display("С02",mq135.readCO2(),"ppm",0,0,0) ;
         delay(refresh_display);
     } while (lcd.getEncoderState() == eNone);


  else if (selectedMenuItem == mkPressure){ //датчик  BME-280(Давление)
     do
    { 
      sensor_display("Давление",BME280A.readFloatPressure(),"hpa",0,0,1) ; 
      delay(refresh_display);
       
    } while (lcd.getEncoderState() == eNone);
    
  }
    
   
  else if (selectedMenuItem == mktemperature) //датчик  BME-280(температура )
           do{
      sensor_display("Температура",BME280A.readTempC(),"C",0,0,1) ; 
      delay(refresh_display);
       
     } while (lcd.getEncoderState() == eNone);





  else if (selectedMenuItem == mkBack)
           do{
       lcd.print("Fals");
       delay(refresh_display);
     } while (lcd.getEncoderState() == eNone);

}




