/*
 * Project DustSensor_Test
 * Description: Setting up the Grove Dust Sensor for the Farming Systen
 * Author: Kareem Crum
 * Date: 05-14-2021
 */

#include "Adafruit_Sensor.h"
#include "math.h"

//Variables
unsigned long dustTime;
unsigned long duration;
int dustSensor = A0;
float dustSense;
unsigned long lowpulseoccupancy = 0;
float concentration = 0;
float ratio = 0;

// setup() runs once, when the device is first turned on.
void setup()
{
  // Put initialization like pinMode and begin functions here.
  pinMode(dustSensor, INPUT);
  Serial.begin(9600);
  dustTime = millis();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() 
{
  // The core of your code will likely live here.
  duration = pulseIn(dustSensor, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  if ((millis()-dustTime) >= 30000)
  {
    ratio = lowpulseoccupancy/(30000*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    if(concentration > 1){
      dustSense = concentration;
    }
    Serial.printf("Concentration = %f pcs/0.01cf",dustSense);
    lowpulseoccupancy = 0;
    dustTime = millis();
  }
}