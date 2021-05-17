/*
 * Project: Acceleromotor_Test
 * Description: Testing the MPU
 * Author: Kareem Crum
 * Date: 17-MAY-2021
 */

#include <Wire.h>
#include "math.h"

byte accel_x_h, accel_x_L;
int16_t accel_x;
float accelX;

byte accel_y_h, accel_y_L;
int16_t accel_y;
float accelY;

byte accel_z_h, accel_z_L;
int16_t accel_z;
float accelZ;

SYSTEM_MODE (SEMI_AUTOMATIC);

// setup() runs once, when the device is first turned on.
void setup() 
{
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  Wire.begin();

  Wire.beginTransmission(0x68);

  Wire.write(0x6B);
  Wire.write(0);

  Wire.endTransmission(true);

  
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
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

} 

void findAcceleration()
{

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);

}