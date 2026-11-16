#include<Arduino.h>


const int ledPin1 = 16;
const int ledChannel1 = 0;

const int freq = 5000;
const int resolution = 8;

void setup() {
  ledcSetup(ledChannel1, freq, resolution);
  ledcAttachPin(ledPin1, ledChannel1);
}

void loop() {

  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(ledChannel1, dutyCycle);
    delay(15);
  }

  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(ledChannel1, dutyCycle);
    delay(15);
  }
}