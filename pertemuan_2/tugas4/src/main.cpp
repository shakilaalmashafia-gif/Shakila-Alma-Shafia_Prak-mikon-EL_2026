#include <arduino.h>

const int ledMerah = 18;
const int ledKuning = 17;
const int ledHijau = 16;
const int PushBottom = 19;

const int potPin = 34;
const float VCC = 3.3;
const float R_TOTAL = 10000.0;

void setup() {

  Serial.begin(115200);

  pinMode(ledMerah,OUTPUT);
  pinMode(ledKuning,OUTPUT);
  pinMode(ledHijau,OUTPUT);
  pinMode(PushBottom,INPUT_PULLDOWN);

}

void loop() {

  if (digitalRead(PushBottom) == HIGH) {
    digitalWrite(ledMerah, LOW);
    digitalWrite(ledKuning, LOW);
    digitalWrite(ledHijau, LOW);
  }

   else {

   int nilaiADC = analogRead(potPin);
   float tegangan = (nilaiADC/4095.0) * VCC;

    if (tegangan < 1.0) {
      digitalWrite(ledMerah, HIGH);
      digitalWrite(ledKuning, LOW);
      digitalWrite(ledHijau, LOW);
    }
     else if (tegangan < 2.0) {
       digitalWrite(ledMerah, LOW);
       digitalWrite(ledKuning, HIGH);
       digitalWrite(ledHijau, LOW);
     }
      else {
        digitalWrite(ledMerah, LOW);
        digitalWrite(ledKuning, LOW);
        digitalWrite(ledHijau, HIGH);
      }
  }

  delay(1000);
}
    
