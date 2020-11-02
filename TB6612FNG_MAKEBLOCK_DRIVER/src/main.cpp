#include <Arduino.h>
#include <GyverEncoder.h>

#define PWMA 6  // выходы arduino
#define PWMB 3
#define AIN1 7
#define AIN2 2
#define BIN1 8
#define BIN2 9


void setup() {
 Serial.begin(9600);

 pinMode(A5,OUTPUT);// int1
 pinMode(A2,OUTPUT);// int2
 pinMode(A4,OUTPUT);//управление оптопарами int3 
 pinMode(13,OUTPUT);//int4


 //pinMode(10,OUTPUT);//step шагового двигателя 
 
 
 pinMode(11,INPUT); //концевой выключатель максимального положения 
 pinMode(12,INPUT);//концевой выключатель нулевого положения 
 pinMode(A0,INPUT);//потенциометр 

    pinMode(PWMA, OUTPUT);  
    pinMode(PWMB, OUTPUT);  
    pinMode(AIN1, OUTPUT);  
    pinMode(AIN2, OUTPUT);  
    pinMode(BIN1, OUTPUT);  
    pinMode(BIN2, OUTPUT); 




}



int motorSpeed = 100; 

void loop() {
 
if(digitalRead(11)==0){
/*
     digitalWrite(AIN1, LOW);      // крутим моторы в одну сторону
     digitalWrite(AIN2, LOW);    
     digitalWrite(BIN1, LOW);  
     digitalWrite(BIN2, LOW); 
     analogWrite(PWMB, 1); 
  */
  Serial.println("stopMax");
     digitalWrite(A5,1);
     delay(100);
     digitalWrite(A5,0);
    
     digitalWrite(AIN1, HIGH);  
     digitalWrite(AIN2, LOW); 
     digitalWrite(BIN1, HIGH);    // крутим моторы в вниз
     digitalWrite(BIN2, LOW);
     
     analogWrite(PWMB, motorSpeed); 

   
     delay(1000);

}


if(digitalRead(12)==0){
   /*  digitalWrite(AIN1, LOW);      // крутим моторы в одну сторону
     digitalWrite(AIN2, LOW);    
     digitalWrite(BIN1, LOW);  
     digitalWrite(BIN2, LOW); 
     analogWrite(PWMB, 1); 
    */

  Serial.println("stopMIN");
     digitalWrite(A2,1);
     delay(100);
     digitalWrite(A2,0);

     digitalWrite(AIN1, LOW);      // крутим моторы в верх
     digitalWrite(AIN2, HIGH);    
     digitalWrite(BIN1, LOW);      
     digitalWrite(BIN2, HIGH); 
     analogWrite(PWMB, motorSpeed);  
     delay(1000);
  
  

}

if(digitalRead(12)!=0 && digitalRead(11)!=0){
  
    
     
    
    Serial.println("GO");

}









/*
Serial.println(analogRead(A0));
digitalWrite(A5,1);
digitalWrite(A2,1);
digitalWrite(A4,1);
digitalWrite(13,1);
Serial.println("ON");
delay(5000);
digitalWrite(A5,0);
digitalWrite(A2,0);
digitalWrite(A4,0);
digitalWrite(13,0);
Serial.println("OFF");
delay(5000);
*/

  // put your main code here, to run repeatedly:
}