/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Users/kareem/Documents/IOT/Smart-Farming-System/Main_Test_Code/src/Main_Test_Code.ino"
/*
 * Project Main_Test_Code
 * Description: The Code branch where we combine all of our code before finalizing it
 * Author: Kareem Crum
 * Date: 05-14-2021
 */
#include <TinyGPS++/TinyGPS++.h>
#include <Adafruit_MQTT.h>

#include "math.h"
#include "JsonParserGeneratorRK.h"
#include "Adafruit_MQTT/Adafruit_MQTT.h" 
#include "Adafruit_MQTT/Adafruit_MQTT_SPARK.h" 
#include "Adafruit_MQTT/Adafruit_MQTT.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Particle.h"
#include "Stepper.h"
#include "credentials.h"

void setup();
void loop();
void helloWorld();
void findAcceleration();
void displayInfo();
void createEventPayload(float jlat, float jlon, float jalt, int sat);
void printValues();
void MQTT_connect();
#line 23 "c:/Users/kareem/Documents/IOT/Smart-Farming-System/Main_Test_Code/src/Main_Test_Code.ino"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    D4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define XPOS 0
#define YPOS 1
#define echoPin D6 // attach pin D6 Arduino to pin Echo of HC-SR04
#define trigPin D7 //attach pin D7 Arduino to pin Trig of HC-SR04



//Global State
TCPClient TheClient;

//Adafruit BME class
Adafruit_BME280 bme;

//Adafruit OLED Class
Adafruit_SSD1306 display(OLED_RESET);

//Stepper Class
Stepper myStepper(2048, D5, D3, D4, D2);

//MQTT Client Class
Adafruit_MQTT_SPARK mqtt(&TheClient,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY); 

//TinyGPS++ Object
TinyGPSPlus gps;

//Feeds
Adafruit_MQTT_Publish Trigger = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/UltraSonic");
Adafruit_MQTT_Publish Satellites = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Satellites");
Adafruit_MQTT_Publish GPS = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Location");
Adafruit_MQTT_Publish Moist = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/SoilMoisture");
Adafruit_MQTT_Publish Pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Pressure");
Adafruit_MQTT_Publish Humid = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Dust = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/DustSensor");
Adafruit_MQTT_Publish FallSense = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Acceleration");


//Variable for BMe
float tempC;
float humidRH;
float pressPA;
float tempF;
float inHg;

//Variables for Soil Sensor
int soilSensor = A1;
int soilMoisturePercent; 
int soilMoistureValue; 

//Variables for DustSensor
unsigned long dustTime;
unsigned long duration;
unsigned long lastMQTT;
unsigned long lastPub;
int dustSensor = A0;
float dustSense;
unsigned long lowpulseoccupancy = 0;
float concentration = 0;
float ratio = 0;

//Variables for UltraSonic Sensor
long amount; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

//Variables for Stepper and OLED
const int stepsRev = 2048;
int16_t stepperSpeed=15;
int16_t stepperSteps;

//Variables for GPS
const unsigned long PUBLISH_PERIOD = 120000;
const unsigned long SERIAL_PERIOD = 5000;
const unsigned long MAX_GPS_AGE_MS = 10000; // GPS location must be newer than this to be considered valid
const int UTC_offset = -6; 
unsigned long lastSerial = 0;
unsigned long lastPublish = 0;
unsigned long startFix = 0;
bool gettingFix = false;

float lat,lon,alt;

//Variables for Acceleromotor
byte accel_x_h, accel_x_L;
int16_t accel_x;
float accelX;

byte accel_y_h, accel_y_L;
int16_t accel_y;
float accelY;

byte accel_z_h, accel_z_L;
int16_t accel_z;
float accelZ;
float accelTot;

bool MPUfall = false;
float MPUValue = 0.0;
const float fall = 1.5;

SYSTEM_MODE(SEMI_AUTOMATIC);

// setup() runs once, when the device is first turned on.
void setup() 
{
  // Put initialization like pinMode and begin functions here.
  pinMode(soilSensor, INPUT);
  pinMode(dustSensor, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  dustTime = millis();

  Serial.begin(9600);

//Acceleromotor Setup
  Wire.begin();

  Wire.beginTransmission(0x68);

  Wire.write(0x6B);
  Wire.write(0);

  Wire.endTransmission(true);

//GPS Setup
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

  bool status;
  status = bme.begin(0x76);
  myStepper.setSpeed(15);
  myStepper.step(0);

  Serial.printf("Connecting to Internet \n");
  WiFi.connect();
  while(WiFi.connecting()) 
  {
    Serial.printf(".");
    delay(100);
  }
  Serial.printf("\n Connected!!!!!! \n");

  //UltraSonic Sensor Setup
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Argon");

}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // The core of your code will likely live here.

  //Code for Stepper Motor
    myStepper.setSpeed(7);
    myStepper.step(1024);
    delay(2000);
    myStepper.step(-1024);
    delay(2000);

  //Code for Dust Sensor
  duration = pulseIn(dustSensor, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  if ((millis()-dustTime) >= 30000)
  {
    ratio = lowpulseoccupancy/(30000*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    if(concentration > 1){
      dustSense = concentration;
    }
    lowpulseoccupancy = 0;
    dustTime = millis();
  }
  
  //Code for Soil Sensor
  soilMoistureValue = analogRead(soilSensor);
  Serial.printf("Soil moisture is %i\n", soilMoistureValue);
  soilMoisturePercent = map(soilMoistureValue, 1800, 3500, 100, 0);
 
 //Code for UltraSonic Sensor
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  amount = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = amount * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

 //Code for BME
  tempC = bme.readTemperature();
  tempF = map(tempC,0.0,100.0,32.0,212.0);
  pressPA = bme.readPressure();
  inHg = pressPA/3386.389;
  humidRH = bme.readHumidity();
  
  //Code for Acceleromotor
  float accelZ, accelX, accelY;
  delay(1000);
  findAcceleration();
  
  Wire.requestFrom(0x68, 6, true);
  accel_x_h = Wire.read();
  accel_x_L = Wire.read();
  accel_x = accel_x_h << 8 |accel_x_L;
  accelX = accel_x / 15000.0;
  Serial.printf("X-axis acceleration is %0.2f\n", accelX);

  accel_y_h = Wire.read();
  accel_y_L = Wire.read();
  accel_y = accel_y_h <<8 | accel_y_L;
  accelY = accel_y / 16000.0;
  Serial.printf("Y-axis acceleration is %0.2f\n", accelY);

  accel_z_h = Wire.read();
  accel_z_L = Wire.read();
  accel_z = accel_z_h << 8 | accel_z_L;
  accelZ = accel_z / 15250.0;
  Serial.printf("Z-axis acceleration is %0.2f\n", accelZ);
  
  accelTot = sqrt(pow(accelX,2)+pow(accelY,2)+pow(accelZ,2));
  Serial.printf("Accel Total Value: %0.2f \n", accelTot);

  if(!MPUfall) 
  {
    MPUValue = accelTot;
    MPUfall = (MPUValue > fall);
  }

  //Code for GPS
   while (Serial1.available() > 0)
  {
    if (gps.encode(Serial1.read()))
	 { 
      displayInfo();
   }
  }
  delay(1000);



  printValues();
  MQTT_connect();
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

void findAcceleration()
{

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);

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
      createEventPayload(lat, lon, alt, sat);
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
void createEventPayload(float jlat, float jlon, float jalt, int sat)
{
char buffer[50];

  sprintf(buffer, "{\"lat\":%0.6f,\"lon\":%0.6f,\"ele\":%0.2f}", jlat, jlon, jalt);

    //Code for Adafruit.IO
    if((millis()-lastPub > 30000)) 
    {
      if(mqtt.Update()) 
      {
        Serial.printf("Publishing %s\n", buffer);
        GPS.publish(buffer);
        Moist.publish(soilMoisturePercent);
        Temp.publish(tempF);
        Humid.publish(humidRH);
        Dust.publish(dustSense);
        Pressure.publish(inHg);  
        Satellites.publish(sat);
        Trigger.publish(distance);
        if(MPUfall)
        {
          FallSense.publish(MPUValue);
          MPUfall = false;
          MPUValue = 0.0;
        }
      } 
      lastPub = millis();
    }  
  
}
void printValues() 
{
    Serial.printf("Temperature = %f\n", tempF);
    Serial.print(bme.readTemperature());
    Serial.println(" *F");
    
    Serial.printf("Pressure = %f\n", inHg);
    Serial.print(bme.readPressure());
    Serial.println(" hPa");

    Serial.printf("Humidity = %f\n", humidRH);
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    
    Serial.printf("Moist: %i %%\n", soilMoisturePercent);
    Serial.printf("Dust: %0.2f\n", dustSense);

    Serial.println();
}
void MQTT_connect() 
{
  int8_t ret;
 
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
 
  Serial.print("Connecting to MQTT... ");
 
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
    // Ping MQTT Broker every 2 minutes to keep connection alive
  if ((millis()-lastMQTT)>120000) 
  {
      Serial.printf("Pinging MQTT \n");
      if(! mqtt.ping()) {
        Serial.printf("Disconnecting \n");
        mqtt.disconnect();
      }
      lastMQTT = millis();
  }
}
