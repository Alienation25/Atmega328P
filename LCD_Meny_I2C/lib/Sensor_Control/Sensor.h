
#include <Max44009.h>
#include <SparkFunBME280.h>


struct I2C_Address_Sensor{

    uint8_t luxA_Address;
    uint8_t luxB_Address;
    uint8_t Press_Address;

};




class Sensor
{
public:
  Sensor::Sensor(I2C_Address_Sensor ){
   

  }


private:
  
     I2C_Address_Sensor *settingI2C;

 

};