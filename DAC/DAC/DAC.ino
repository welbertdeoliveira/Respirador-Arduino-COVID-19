#include <Arduino.h>
//#include <Waveform.h>


void setup() {
  analogWriteResolution(12);
  
  // put your setup code here, to run once:

}

void loop() {
  analogWrite(DAC0,2048 );
  analogWrite(2,2048 );
  delay(600);
  analogWrite(DAC0,0);
  analogWrite(2,0);
  delay(50);
  analogWrite(DAC0,2606);
  analogWrite(2,2606);
  delay(50);
  analogWrite(DAC0,1614 );
  analogWrite(2,1614);
  delay(100);
  
  // put your main code here, to run repeatedly:

}
