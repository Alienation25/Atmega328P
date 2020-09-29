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
#define refresh_display 100 
//
//Пины модулей
//
#define PIN_MQ135  A0 //анализ газа (жёлтый) 
#define PIN_Capacitive_Soil_Sensor1 A1 //анализатор грунта (оранжевый) 
#define PIN_AM2301A 9 // Температуро и влага анализатор (бордовый)
#define PIN_DS18 10 // два датчика температуры (черный) 
#define PIN_AM2301B 11 // Температуро и влага анализатор (белый)
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
int language=1; //язык интерфейса
//
//
//
long lastUpdateTime = 0; // Переменная для хранения времени последнего считывания с датчика
const int TEMP_UPDATE_TIME = 1000; // Определяем периодичность проверок
int temperature=0; //буферная переменая для хранения значения

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




void checkAddress( byte addr [2][8],OneWire ds)//поиск адресов (2 элемента) , шина oneWireA 10 
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

// Описание меню
// структура пункта меню: {ParentKey, Key, Caption, [Handler]}
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

uint8_t menuLen = sizeof(menuRussion) / sizeof(sMenuItem);//Структура для хранения значений датчиков 

bool work;
void setup() {
  Serial.begin(9600);//запуск UART(RX TX)

  lcd.begin();//запуск экрана 
  lcd.attachEncoder(pinDT, pinCLK, pinSW);//запуск энкодера
  lcd.printAt(0,0,"ШКОЛА РОБОТОТЕХНИКИ");
  lcd.printAt(0,1,"Вперед в Будущее");
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
        sensor_display("Температура",am2301b.readTemperature(),"С",0,0,1) ; 
        sensor_display("Влажность",am2301b.readHumidity(),"%",0,1,1) ; 
        //sensor_display("Влага",BME280A.readFloatHumidity(),"%",0,1,1) ; //датчик  BME-280(влажности)
        delay(refresh_display);
       
    } while (lcd.getEncoderState() == eNone);
  }



 else if (selectedMenuItem == mktemperatureUp){//DS18 (Датчик температуры )
    do
    {
        detectTemperature(ds18[0]);   
        sensor_display("Температура",temperature,"C",0,0,0);
        //sensor_display("Температура",BME280A.readTempC(),"C",0,0,1) ; //датчик  BME-280(температура) 
        delay(refresh_display);
    
    } while (lcd.getEncoderState() == eNone);
 }


   else if (selectedMenuItem == mktemperatureDown){//DS18 (Датчик температуры )
    do
    {
      
         detectTemperature(ds18[1]);   
         sensor_display("Температура",temperature,"C",0,0,0);
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




