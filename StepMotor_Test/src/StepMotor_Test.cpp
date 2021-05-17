/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/AFYAW/OneDrive/Documents/IOT/Smart-Farming-System/StepMotor_Test/src/StepMotor_Test.ino"
/*
 * Project L16_05_StepperGyro
 * Description: Wire stepper motor and move motor 2 rotations clockwise, pause, move 1 rotation counter-clockwise, repeat
 * Author:Stephanie Perea
 * Date:05/16/2021
*/

#include <Wire.h>
#include <math.h>
#include <Stepper.h>


 // Declare variables
void setup();
void loop();
#line 14 "c:/Users/AFYAW/OneDrive/Documents/IOT/Smart-Farming-System/StepMotor_Test/src/StepMotor_Test.ino"
byte accel_x_h,accel_x_l; // variables to store the individual btyes
byte accel_y_h,accel_y_l; // variables to store the individual bytes
byte accel_z_h,accel_z_l; // variables to store the individual bytes

int16_t accel_x; // variable to store the x- acceleration
int16_t accel_y; // variable to store the y- acceleration
int16_t accel_z; // variable to store the z- acceleration

//int16_t myStepper;
int16_t stepper_in1 = D1; //variable to store info from IN1
int16_t stepper_in2 = D2; //variable to store info from IN2
int16_t stepper_in3 = D3; //variable to store info from IN3
int16_t stepper_in4 = D4; //variable to store info from IN4 
int16_t stepperSpeed=15;
int16_t stepperSteps;
int16_t spr=2048;
Stepper myStepper (2048, D1,D3, D2, D4);


float ax=-16236.0;
float ay=15648.0;
float az=15612.0;
float SFx=-1/ax;
float SFy=-1/ay;
float SFz=-1/az;
float axg;
float ayg;
float azg;
float rpm = 1-15;
float speed=rpm;
SYSTEM_MODE(SEMI_AUTOMATIC);
// setup() runs once, when the device is first turned on.
void setup() {
  //Begin I2C communications
  Wire.begin();

  //Begin transmission to MPU-6050
  Wire.beginTransmission(0x68);

  // Select and write to PWR_MGMT1 register
Wire.write (0x6B ) ;
Wire.write (0) ; // set to 0 ( wakes up MPU -6050)

// End transmission and close connection
Wire.endTransmission (true) ;
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
// Set the " pointer " to the 0x3B memory location of the MPU and wait for data
Wire.beginTransmission (0x68);
Wire.write (0x3B); // starting with register 0x3B
Wire.endTransmission (false); // keep active .

// Request and then read 2 bytes
// Syntax :
// Wire.requestFrom (I2C_addr, quantity, stop);
// Wire.read (); // repeat this for each byte to be read

Wire.requestFrom (0x68, 6, true);
accel_x_h = Wire.read (); // x accel MSB
accel_x_l = Wire.read (); // x accel LSB
accel_y_h = Wire.read (); // y accel MSB
accel_y_l = Wire.read (); // y accel LSB
accel_z_h = Wire.read (); // z accel MSB
accel_z_l = Wire.read (); // z accel LSB

accel_x = accel_x_h << 8|accel_x_l; // i = 2 bits not high of x - what happens if declared int instead ? negatives wont work
accel_y = accel_y_h << 8|accel_y_l;
accel_z = accel_z_h << 8|accel_z_l;

//formula to calibrate
axg=accel_x*SFx;
ayg=accel_y*SFy;
azg=accel_z*SFz;

Serial.printf ("X- axg %f \n", axg);
delay(3000);
Serial.printf ("Y- ayg %f \n", ayg);
delay(3000);
Serial.printf ("Z- azg %f \n", azg);
delay(3000);

myStepper.setSpeed(15);
myStepper.step(1024);
delay(9000);
myStepper.step(-1024);
delay(5000);
 

}