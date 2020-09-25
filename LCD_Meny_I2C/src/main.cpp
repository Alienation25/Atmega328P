#include <Arduino.h>
// Пример использования меню для построения диалога
#include <Wire.h>
#include <LiquidCrystal_I2C_Menu.h>
#include <Max44009.h>
#include <SparkFunBME280.h>
#include <TroykaMQ.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


#include <OneWire.h>
#include <DallasTemperature.h>



// Пины, к которым подключен энкодер
#define pinCLK 2
#define pinDT  3
#define pinSW  4
///
///Обновление єкрана
#define refresh_display 100 
//
//Пины модулей
//
#define PIN_MQ135  A0 //анализ газа жёлтый 
#define PIN_Capacitive_Soil_Sensor1 A1 //анализатор грунта оранжевый 
#define PIN_Capacitive_Soil_Sensor2 A2 //анализатор грунта красный
#define PIN_AM2301 9 // Температуро анализатор бордовый
#define PIN_DS18 10 // два датчика температуры черный 
  


LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);
BME280 BME280A(0x76);
Max44009 LuxA(0x4A);
Max44009 LuxB(0x4B);
MQ135 mq135(PIN_MQ135);
DHT am2301(PIN_AM2301, DHT21);



OneWire oneWireA(PIN_DS18);
long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок
int temperature=0;


byte ds18A[8] = {0x28, 0x38, 0xE,0x5C, 0x1A, 0x19, 0x1,0x1A };
byte ds18B[8] = {0x28, 0xAD, 0x62 ,0x41,0x1A, 0x19,0x1,0xD5 };


int detectTemperature(byte addres[8]){  //Функция для работы с датчиками

  byte data[2];
  oneWireA.reset();
  oneWireA.select(addres);
  oneWireA.write(0x44);

  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    delay(10);
    lastUpdateTime = millis();
    oneWireA.reset();
    oneWireA.select(addres);
    oneWireA.write(0xBE);
    data[0] = oneWireA.read();
    data[1] = oneWireA.read();

    // Формируем значение
    temperature = (data[1] << 8) + data[0]; 
    temperature = temperature >> 4;
  }
}









uint8_t language=1; //язык интерфейса


// Объявим перечисление, используемое в качестве ключа пунктов меню
enum {Russion,English};//потдержка языков
enum {mkRoot,mkBack, mkLux, mkC02, mkPressure,mktemperatureAirHumidity ,mktemperatureUp,mktemperatureDown,mkcapacitiveSoil,mklanguage};

//printWL("Датчик света","Sensor light").

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}

//printWL("Датчик света","Light sepnsor")
sMenuItem menuRussion[] = {
    {mkBack, mkLux,"Датчик света"},
    {mkBack, mkC02, "Датчик С02"},
    {mkBack, mkPressure,"Датчик давления"},
    {mkBack, mktemperatureUp,"Температура верх"},
    {mkBack, mktemperatureDown,"Температура низ"},
    {mkBack, mktemperatureAirHumidity,"Температуры и влажность воздуха"},
    {mkBack, mkcapacitiveSoil,"Влажность почвы"},
    //{mkBack, mklanguage,"Язык"},
    {mkRoot, mkBack, "Exit menu"}
};






uint8_t menuLen = sizeof(menuRussion) / sizeof(sMenuItem);

//Структура для хранения значений датчиков 






void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.attachEncoder(pinDT, pinCLK, pinSW);
  am2301.begin();
  mq135.calibrate(); 
  Serial.print("MQ135 resistor = ");
  Serial.println(mq135.getRo());

  


  if (BME280A.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
  }
}



void sensor_display(String name,float number_sensor,String unit,char r,char c,char accuracy){//функция для вывлда показаний датчика
    String buffer= String(number_sensor,accuracy);       
    lcd.printfAt(r,c,"%s=%s%s          ",name.c_str(),buffer.c_str(),unit.c_str());
    
   
}





void loop() {
  // Показываем меню
  
  uint8_t selectedMenuItem = lcd.showMenu(menuRussion, menuLen, 0);
 
  
  // И выполняем действия в соответствии с выбранным пунктом
  
  if (selectedMenuItem == mkLux){  //два датчика GY-49(датчики света)
      do
      {
        sensor_display("Свет",LuxA.getLux(),"лм",0,0,0) ; 
        sensor_display("Свет",LuxB.getLux(),"лм",0,1,0) ; 
        delay(refresh_display);
    
       } while (lcd.getEncoderState() == eNone);
    }
 
   
  else if (selectedMenuItem == mkC02){ //С02 (датчик угарного газа) 
     do
     {  
        sensor_display("C02",mq135.readCO2(),"ppm",0,0,0) ;
        delay(refresh_display);
    
     } while (lcd.getEncoderState() == eNone);
  }



  else if (selectedMenuItem == mkPressure){ //датчик  BME-280(Давление)
     do
     {

       sensor_display("Давление",BME280A.readFloatPressure(),"hpa",0,0,1) ; 
       delay(refresh_display);
    
     } while (lcd.getEncoderState() == eNone);
    
  }
    
  else if (selectedMenuItem == mktemperatureAirHumidity){//DH21 (AM2301)(датчик температуры и влажности)
    do
    {
        sensor_display("Температура",am2301.readTemperature(),"С",0,0,1) ; 
        sensor_display("Влажность",am2301.readHumidity(),"%",0,1,1) ; 
        //sensor_display("Влага",BME280A.readFloatHumidity(),"%",0,1,1) ; //датчик  BME-280(влажности)
        delay(refresh_display);
       
    } while (lcd.getEncoderState() == eNone);
  }



 else if (selectedMenuItem == mktemperatureUp){
    do
    {
        detectTemperature(ds18A);   
        sensor_display("Температура",temperature,"C",0,0,0);
        //sensor_display("Температура",BME280A.readTempC(),"C",0,0,1) ; //датчик  BME-280(температура) 
        delay(refresh_display);
    
    } while (lcd.getEncoderState() == eNone);
 }


   else if (selectedMenuItem == mktemperatureDown){
    do
    {
      
         detectTemperature(ds18B);   
         sensor_display("Температура",temperature,"C",0,0,0);
         //sensor_display("Температура",BME280A.readTempC(),"C",0,0,1) ; //датчик  BME-280(температура) 
         delay(refresh_display);
       
        
    
    } while (lcd.getEncoderState() == eNone);
 }

  else if (selectedMenuItem == mkcapacitiveSoil){
    do
    {
     
         sensor_display("Влажность",analogRead(PIN_Capacitive_Soil_Sensor1),"%",0,0,0);
         sensor_display("Влажность",analogRead(PIN_Capacitive_Soil_Sensor2),"%",0,1,0);
         
    
    } while (lcd.getEncoderState() == eNone);
 }




  else if (selectedMenuItem == mkBack){
    do
    {

        lcd.print("Fals");
        delay(refresh_display);

     } while (lcd.getEncoderState() == eNone);

  }



}




