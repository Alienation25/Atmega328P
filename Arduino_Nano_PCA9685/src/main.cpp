#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

int pin =0;// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

void setup() {
  Serial.begin(9600);
  Serial.println("GPIO test!");

  pwm.begin();
  pwm.setPWMFreq(1000);  // Set to whatever you like, we don't use it in this demo!

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  for (int pin=0;pin<=16;pin++)
  pwm.setPWM(pin,4096,0);//off

  Wire.setClock(400000);
}

void loop() {
  // Drive each pin in a 'wave'
    for (int pin=0;pin<=16;pin++){
    delay(3000);
     pwm.setPWM(pin, 0, 4096);     // turns pin fully on
    delay(3000);
     pwm.setPWM(pin, 4096, 0);    // turns pin fully off
    }
  
}