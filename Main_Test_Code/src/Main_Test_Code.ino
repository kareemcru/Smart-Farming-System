/*
 * Project Main_Test_Code
 * Description: The Code branch where we combine all of our code before finalizing it
 * Author: Kareem Crum
 * Date: 05-14-2021
 */

#include <Wire.h>
#include <math.h>
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "Stepper.h"

//Adafruit BME class
Adafruit_BME280 bme;

//Adafruit OLED Class
Adafruit_SSD1306 display(OLED_RESET);

//Stepper Class
Stepper myStepper(stepsRev, D5, D3, D4, D2);

#define OLED_RESET D4
#define SCREEN_ADDRESS 0x3C
#define XPOS 0
#define YPOS 1
#define OLED_RESET D4
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04


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
  bool status;
  status = bme.begin(0x76);
  myStepper.setSpeed(15);

}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // The core of your code will likely live here.

  //Code for Stepper Motor
  myStepper.step(4096);
  Serial.println("cloockwise");
  delay(1000);

  myStepper.step(-2048);
  Serial.println("counterclockwise");


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
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  amount = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

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

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");
    
    Serial.printf("Moist: %i %%\n", soilMoisturePercent);
    Serial.printf("Dust: %0.2f\n", dustSense);

    Serial.println();
}