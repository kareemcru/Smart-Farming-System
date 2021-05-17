/*
 * Project GPS_Test_05-12-2021
 * Description: Testing out the GPS unit
 * Author: Kareem Crum
 * Date: MAY-12-2021
 */

#include <TinyGPS++/TinyGPS++.h>
#include "Particle.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    D4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


//OLED Object
Adafruit_SSD1306 display(OLED_RESET);
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

const unsigned long PUBLISH_PERIOD = 120000;
const unsigned long SERIAL_PERIOD = 5000;
const unsigned long MAX_GPS_AGE_MS = 10000; // GPS location must be newer than this to be considered valid

// The TinyGPS++ object
TinyGPSPlus gps;
const int UTC_offset = -6; 
unsigned long lastSerial = 0;
unsigned long lastPublish = 0;
unsigned long startFix = 0;
bool gettingFix = false;

float lat,lon,alt;


SYSTEM_MODE(SEMI_AUTOMATIC);

void setup()
{
	Serial.begin(9600);

	Serial1.begin(9600);
	startFix = millis();
	gettingFix = true;

	Serial.println(F("DeviceExample.ino"));
	Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
	Serial.print(F("Testing TinyGPS++ library v. ")); 
	Serial.println(TinyGPSPlus::libraryVersion());
	Serial.println(F("by Mikal Hart"));
	Serial.println();

	//OLED Setup
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	helloWorld();
	Serial.printf("Hello World \n");
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (Serial1.available() > 0)
  {
    if (gps.encode(Serial1.read()))
	{ 
      displayInfo();
	}
  }
  delay(1000);
}
																																														
void displayInfo()
{
	float lat,lon,alt;
	uint8_t hr,mn,se,sat;
	if (millis() - lastSerial >= SERIAL_PERIOD)
	{
		lastSerial = millis();

		char buf[128];
		if (gps.location.isValid() && gps.location.age() < MAX_GPS_AGE_MS) 
		{
			lat = gps.location.lat();
			lon = gps.location.lng(); 
			alt = gps.altitude.meters();
			hr = gps.time.hour();
			mn = gps.time.minute();
			se = gps.time.second();
			sat = gps.satellites.value();

			if(hr > 7) 
			{
				hr = hr + UTC_offset;
			}
			else 
			{
				hr = hr + 24 + UTC_offset;
			}
			Serial.printf("Time: %02i:%02i:%02i --- ",hr,mn,se);
			Serial.printf("lat: %f, long: %f, alt: %f \n", lat,lon,alt);
			if (gettingFix) 
			{
				gettingFix = false;
				unsigned long elapsed = millis() - startFix;
				Serial.printlnf("%lu milliseconds to get GPS fix", elapsed);
			}
			display.clearDisplay();
			display.setCursor(0,0);
			display.printf("Time: %02i:%02i:%02i \n",hr,mn,se);
			display.printf("lat  %f \nlong %f \nalt %f\n", lat,lon,alt);
			display.printf("satelites %i", sat);
			display.display();
		}
		else 
		{
			strcpy(buf, "no location");
			if (!gettingFix) 
			{
				gettingFix = true;
				startFix = millis();
			}
		}
	}
}
void helloWorld() 
{
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(20,5);
	display.println("GPS Initializing");
	display.display();
}