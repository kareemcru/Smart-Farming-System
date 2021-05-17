/*
 * Project L16_05_StepperGyro
 * Description: Wire stepper motor and move motor 2 rotations clockwise, pause, move 1 rotation counter-clockwise, repeat
 * Author:Stephanie Perea
 * Date:05/16/2021
*/

#include <math.h>
#include <Stepper.h>


//int16_t myStepper;
int16_t stepper_in1 = D5; //variable to store info from IN1
int16_t stepper_in2 = D4; //variable to store info from IN2
int16_t stepper_in3 = D3; //variable to store info from IN3
int16_t stepper_in4 = D2; //variable to store info from IN4 
int16_t stepperSpeed=15;
int16_t stepperSteps;
int16_t spr=2048;
Stepper myStepper (2048, D1,D3, D2, D4);

SYSTEM_MODE(SEMI_AUTOMATIC);
// setup() runs once, when the device is first turned on.
void setup() 
{
  myStepper.step(0);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
 {
myStepper.setSpeed(15);
myStepper.step(1024);
delay(9000);
myStepper.step(-1024);
delay(5000);
 

}