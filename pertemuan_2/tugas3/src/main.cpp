#include <arduino.h>

const int potPin = 34;
const float VCC = 3.3;
const float R_TOTAL = 10000.0;

void setup() {
  Serial.begin(115200);

  //ADC ESP32 = 12-bit 
  analogReadResolution(12);
}

void loop() {
  //Baca nilai ADC mentah
  int rawADC = analogRead(potPin);

  //Konversi ADC menjadi tegangan
  float voltage = (rawADC / 4095.0) * VCC;

  //Hitung hambatan
  float resistance = (voltage / VCC) * R_TOTAL;

  //Tampilkan hasil
  Serial.print("Raw ADC     : ");
  Serial.println(rawADC);

  Serial.print("Tegangan    : ");
  Serial.print(voltage, 2);
  Serial.println(" V");

  Serial.print("Hambatan    : ");
  Serial.print(resistance, 2);
  Serial.println(" Ohm");

  delay(500);
}
