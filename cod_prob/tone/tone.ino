#include  <Arduino.h>

int aux = 0;
void setup() {

  pinMode (A0, INPUT);
  pinMode (30, OUTPUT);
    
  // put your setup code here, to run once:

}

void loop() {
  aux = map(analogRead(A0),0,1023,100,10000);
  tone(30, aux);
  // put your main code here, to run repeatedly:

}
