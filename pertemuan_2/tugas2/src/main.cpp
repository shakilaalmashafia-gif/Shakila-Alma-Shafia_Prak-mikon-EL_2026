#include <arduino.h>

const int ledMerah = 18;
const int ledKuning = 17;
const int ledHijau = 16;
const int PushBottom = 19;

void setup() {

   pinMode(PushBottom, INPUT_PULLDOWN);
   pinMode(ledMerah, OUTPUT);
   pinMode(ledKuning, OUTPUT);
   pinMode(ledHijau, OUTPUT);
   
}

void loop() {
    if (digitalRead(PushBottom) == HIGH) {
      digitalWrite(ledMerah, HIGH);
      digitalWrite(ledKuning, HIGH);
      digitalWrite(ledHijau, HIGH);
    } 
}