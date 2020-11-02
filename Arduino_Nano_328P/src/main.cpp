#include <Arduino.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  for(int i=4;i<=7;++i){
 pinMode(i,OUTPUT);
}
 
}

// the loop function runs over and over again forever
void loop() {
  

 for(int i=4;i<=7;++i){
 digitalWrite(i,HIGH);
 delay(1000);
}

 
 for(int i=4;i<=7;++i){
 digitalWrite(i,LOW);
 delay(1000);
}
 
 

                       // wait for a second
}