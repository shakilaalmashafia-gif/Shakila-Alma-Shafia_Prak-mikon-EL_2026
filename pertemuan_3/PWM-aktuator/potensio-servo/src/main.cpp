#include <arduino.h>

const int potPin = 18;
const int servoPin = 16;
const int freq = 50;
const int channel = 0;
const int res = 12;

void setup() {
  Serial.begin(115200);
   ledcSetup(channel, freq, res);
   ledcAttachPin(servoPin, channel);
}

void loop() {
  int potValue = analogRead(potPin); // Hasil: 0 - 4095
  int pwmValue = map(potValue, 0, 4095, 0, 255); // Konversi ke 0 - 255
  Serial.println(pwmValue);
  ledcWrite (channel,pwmValue);

}

