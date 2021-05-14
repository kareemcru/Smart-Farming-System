/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/kareem/Documents/IOT/Smart-Farming-System/BME_SoilSensor_Test/src/BME_SoilSensor_Test.ino"
/*
 * Project BME_SoilSensor_Test
 * Description: Setting up the BME and Soil Sensor for Smart Farm
 * Author: Kareem Crum
 * Date: 05-14-2021
 */

#include "Adafruit_BME280.h"

//Adafruit BME class
void setup();
void loop();
void printValues();
#line 11 "c:/Users/kareem/Documents/IOT/Smart-Farming-System/BME_SoilSensor_Test/src/BME_SoilSensor_Test.ino"
Adafruit_BME280 bme;

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


SYSTEM_MODE(SEMI_AUTOMATIC);

// setup() runs once, when the device is first turned on.
void setup() 
{
  // Put initialization like pinMode and begin functions here.
  pinMode(soilSensor, INPUT);
  Serial.begin(9600);
  bool status;
  status = bme.begin(0x76);

}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // The core of your code will likely live here.
  
  //Code for Soil Sensor
  soilMoistureValue = analogRead(soilSensor);
  Serial.printf("Soil moisture is %i\n", soilMoistureValue);
  soilMoisturePercent = map(soilMoistureValue, 1800, 3500, 100, 0);
 
 //Code for BME
  tempC = bme.readTemperature();
  tempF = map(tempC,0.0,100.0,32.0,212.0);
  pressPA = bme.readPressure();
  inHg = pressPA/3386.389;
  humidRH = bme.readHumidity();

  printValues();

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
    
    Serial.printf("Moist: %i %%\n", soilMoisturePercent);

    Serial.println();
}