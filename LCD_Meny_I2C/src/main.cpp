#include <Arduino.h>// библиотека для работы с Arduin nano
#include <Wire.h> //библиотека для работы с I2C
#include <LiquidCrystal_I2C_Menu.h>//библиотека для работы с экраном и меню
#include <Max44009.h> // библиотека для работы с датчиком света 
#include <SparkFunBME280.h>// библиотека для работы с барометром 
#include <TroykaMQ.h>//библиотека для работы с датчиком C02
#include <Adafruit_Sensor.h>// библиотека для работы с датчиком AM2301(константы)
#include <DHT.h> // библиотека для работы с датчиком AM2301
#include <OneWire.h>// библиотека для работы с гигрометром 

 /*
  Serial.print("MQ135 resistor = "); 
  Serial.println(mq135.getRo());//Вывод сопротивления нагревательного резистора
  */
  

// Пины, к которым подключен энкодер
#define pinCLK 2 
#define pinDT  3 
#define pinSW  4 
///
///Обновление єкрана
#define refresh_display 200
//
//Пины модулей
//
#define PIN_MQ135  A0 //анализ газа (жёлтый) 
#define PIN_Capacitive_Soil_Sensor1 A1 //анализатор грунта (оранжевый) 
#define PIN_AM2301A 9 // Температуро и влага анализатор (бордовый)
#define PIN_AM2301B 11//11 // Температуро и влага анализатор (белый)
#define PIN_DS18 7// первый датчик (6) 

//#define PIN_Capacitive_Soil_Sensor2 A2 //анализатор грунта (красный)
//  
//I2C адреса и классы для работы
//
LiquidCrystal_I2C_Menu lcd(0x27, 16, 2);//экран
BME280 BME280A(0x76);//датчик давления
Max44009 LuxA(0x4A);// датчик освещёности 
Max44009 LuxB(0x4B);// датчик освещёности 
MQ135 mq135(PIN_MQ135);// датчик С02
DHT am2301a(PIN_AM2301A, DHT21);// датчик температуры и влажности
DHT am2301b(PIN_AM2301B, DHT21);// датчик температуры и влажности
OneWire oneWireA(PIN_DS18);  //шина датчиков температуры (2 шт)
//
//Переменые для настройки работы датчика температуры (DS18) в функцие detectTemperature()
//
int language=0; //язык интерфейса
//
//
//
long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок
int temperature=0; //буферная переменая для хранения значения ds18(1)
 //буферная переменая для хранения значения ds18(2)
// Перемненые для хранения адресов датчикво в 16 ричном коде 


byte ds18[2][8];//[2]- два датчика [8] - бит информации






int detectTemperature(byte addres[8]){  //Функция для работы с датчиками DS18
  byte data[2];
  oneWireA.reset();// перезагрузка порта 
  oneWireA.select(addres);//выбор адресса для работы 
  oneWireA.write(0x44); //выбор режима сохранения 

  if (millis() - lastUpdateTime > TEMP_UPDATE_TIME)
  {
    delay(10);
    lastUpdateTime = millis();

    oneWireA.reset();// перезагрузка порта 
    oneWireA.select(addres);//выбор адресса для работы 
    oneWireA.write(0xBE);//читает SPA(блокнотная оперативная память или локальное хранилище )
    data[0] = oneWireA.read();//с него читаем байты
    data[1] = oneWireA.read();//с него читаем байты

    // Формируем значение
    temperature = (data[1] << 8) + data[0]; 
    temperature = temperature >> 4;
  }
}




void checkAddress( byte addr [2][8],OneWire ds)//поиск адресов (2 элемента) , шина oneWireA 7,6 
{
   for(int i=0;i<2;i++){
      if (!ds.search(addr[i])) {
        Serial.println(" No more addresses.");
        ds.reset_search();
        delay(250);
      return;
       }
      
      if (OneWire::crc8(addr[i], 7) != addr[i][7]) {
        Serial.println("CRC is not valid!");
        return;
      }
   }
 ds.reset_search();
}



// Объявим перечисление, используемое в качестве ключа пунктов меню
enum {Russion,English};//потдержка языков
enum {mkRoot,mkBack, mkLux, mkC02, mkPressure,mktemperatureAirHumidity,mktemperatureUp,mktemperatureDown,mkcapacitiveSoil,mklanguage};//Вкладки меню


const char * chengLangu(const char * Rus , const char * Eng){
 
   if(Russion== language)
   {
     return Rus;
     
   }
     if(English == language)
   {
     return Eng;

   }


}



// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
sMenuItem menuRussion[] = {
    {mkBack, mkLux,chengLangu("Датчик света","light sensor")},
    {mkBack, mkC02, chengLangu("Датчик CO2","CO2 sensor")},
    {mkBack, mkPressure,chengLangu("Датчик давления","pressure sensor")},
    {mkBack, mktemperatureUp,chengLangu("Температура верх","temperature top")},
    {mkBack, mktemperatureDown,chengLangu("Температура вниз","temperature down")},
    {mkBack, mktemperatureAirHumidity,chengLangu("Температуры и влажность воздуха","Temperature and humidity")},
    {mkBack, mkcapacitiveSoil,chengLangu("Влажность почвы","Soil moisture")},
    //{mkBack, mklanguage,"Язык"},
    {mkRoot, mkBack, "Exit menu"}
};

uint8_t menuLen = sizeof(menuRussion) / sizeof(sMenuItem);//Структура для хранения значений датчиков 

bool work;
void setup() {
  Serial.begin(9600);//запуск UART(RX TX)
  lcd.begin();//запуск экрана 
  lcd.attachEncoder(pinDT, pinCLK, pinSW);//запуск энкодера
  lcd.printAt(0,0,chengLangu("",""));
  lcd.printAt(0,1,chengLangu("привет","Hello"));
  delay(5000);
  am2301a.begin();//запуск датчика температуры и влажности 
  am2301b.begin();//запуск датчика температуры и влажности 
  mq135.calibrate(); //калибровка датчика
  checkAddress(ds18,oneWireA);//наличие датчиков ds18  
  if (BME280A.beginI2C() == false) //Проверка I2C
  {
    Serial.println("Cенсор не найден.");
  }
}

void temp(){
detectTemperature(ds18[0]); 
Serial.println(temperature);
delay(1000);
}






void sensor_UART(){
    Serial.println("#");
     Serial.println(LuxA.getLux()); //lm  // fl //\n
     Serial.println(LuxB.getLux()); //lm  // fl //\n
     Serial.println(mq135.readCO2());//ppm // ulong
     Serial.println(BME280A.readFloatPressure());//hpa  //fl
     Serial.println(am2301a.readTemperature());//C //fl
     Serial.println(am2301a.readHumidity());//%   //fl
     Serial.println(am2301b.readTemperature());//C //fl
     Serial.println(am2301b.readHumidity());//%  //fl
    temp();
    Serial.println(analogRead(PIN_Capacitive_Soil_Sensor1));//% //i
    
    Serial.println("$");
}




void sensor_display(String name,float number_sensor,String unit,char r,char c,char accuracy){//функция для вывлда показаний датчика
    String buffer= String(number_sensor,accuracy);//буфер для контроля точностью       
    lcd.printfAt(r,c,"%s=%s%s          ",name.c_str(),buffer.c_str(),unit.c_str());//функция для вывода знаений на экран
       
}


void loop() {
  uint8_t selectedMenuItem = lcd.showMenu(menuRussion, menuLen, 0);  // Показываем меню
  // И выполняем действия в соответствии с выбранным пунктом
 
  if (selectedMenuItem == mkLux){  //два датчика GY-49(датчики света)
      do
      {
         sensor_UART();
        sensor_display(chengLangu("Свет","Light"),LuxA.getLux(),chengLangu("лм","lm"),0,0,0) ; 
        sensor_display(chengLangu("Свет","Light"),LuxB.getLux(),chengLangu("лм","lm"),0,0,0) ; 
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

       sensor_display(chengLangu("Давление","Pressure"),BME280A.readFloatPressure(),"hpa",0,0,1) ; 
       delay(refresh_display);
    
     } while (lcd.getEncoderState() == eNone);
    
  }
    
  else if (selectedMenuItem == mktemperatureAirHumidity){//DH21 (AM2301)(датчик температуры и влажности)
    do
    {
        sensor_display(chengLangu("Температура","temperatyre"),am2301b.readTemperature(),"С",0,0,1) ; 
        sensor_display(chengLangu("Влажность","Humidity"),     am2301b.readHumidity()   ,"%",0,1,1) ; 
        //sensor_display("Влага",BME280A.readFloatHumidity(),"%",0,1,1) ; //датчик  BME-280(влажности)
        delay(refresh_display);
       
    } while (lcd.getEncoderState() == eNone);
  }



 else if (selectedMenuItem == mktemperatureUp){//DS18 (Датчик температуры )
    do
    {
        detectTemperature(ds18[0]);   
        sensor_display(chengLangu("Температура","temperatyre"),temperature,"C",0,0,0);
        //sensor_display("Температура",BME280A.readTempC(),"C",0,0,1) ; //датчик  BME-280(температура) 
        delay(refresh_display);
    
    } while (lcd.getEncoderState() == eNone);
 }


   else if (selectedMenuItem == mktemperatureDown){//DS18 (Датчик температуры )
    do
    {
      
         detectTemperature(ds18[1]);   
         sensor_display(chengLangu("Температура","temperatyre"),temperature,"C",0,0,0);
         
         //sensor_display("Температура",BME280A.readTempC(),"C",0,0,1) ; //датчик  BME-280(температура) 
         delay(refresh_display);
  
    } while (lcd.getEncoderState() == eNone);
 }

  else if (selectedMenuItem == mkcapacitiveSoil){//гидрометр 
    do
    {
     
         sensor_display("Влажность",analogRead(PIN_Capacitive_Soil_Sensor1),"%",0,0,0);       
    
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




