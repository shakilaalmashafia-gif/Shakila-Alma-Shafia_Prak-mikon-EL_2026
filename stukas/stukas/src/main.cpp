#include <Arduino.h>
#include <Wire.h> 
#include <Adafruit_INA219.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>

Adafruit_INA219 ina219;
Adafruit_SSD1306 display(128, 64, &Wire, -1); 

//motor DC
const	int	in1	=	17;
const	int	in2	=	18;
const	int	pwmPin	=	16;
const	int	stby	=	19;
const	int	potPin	=	34;
//led
const int ledPin = 2;


void	setup()	{
  Serial.begin(115200);

  //motor DC
		pinMode(in1,	OUTPUT);
		pinMode(in2,	OUTPUT);
		pinMode(stby,	OUTPUT);
  //led
    pinMode(ledPin, OUTPUT);
		
		digitalWrite(stby,	HIGH);	//	Aktifkan	driver
		digitalWrite(in1,	HIGH);		//	Atur	arah	putaran	maju
		digitalWrite(in2,	LOW);
    
		ledcSetup(0,	5000,	8);
		ledcAttachPin(pwmPin,	0);

    Wire.begin();
    ina219.begin();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

}


void	loop()	{
  //pot
  	int	potValue	=	analogRead(potPin);	//	Hasil:	0	-	4095
		int	pwmValue	=	map(potValue,	0,	4095,	0,	255);	//	Konversi	ke	0	-	25

		ledcWrite(0,	200);	//	Set	kecepatan	motor	(0-255)

  //ina
  float arus = ina219.getCurrent_mA(); 
  float tegangan_V = ina219.getBusVoltage_V(); 
  float daya_mW = ina219.getPower_mW(); 
  delay(1000);
  
  //oled
  display.clearDisplay(); 
  display.display();  
}