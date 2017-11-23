
/*
  Grove- i2C motor driver demo v1.0
  by: http://www.seeedstudio.com
*/
//  Author:LG
#include <Wire.h>
#include "MMA7660.h"
MMA7660 accelemeter;

#define MotorSpeedSet             0x82
#define PWMFrequenceSet           0x84
#define DirectionSet              0xaa
#define MotorSetA                 0xa1
#define MotorSetB                 0xa5
#define Nothing                   0x01
#define Stepernu                  0x1c
#define I2CMotorDriverAdd         0x28   // Set the address of the I2CMotorDriver


void MotorSpeedSetAB(unsigned char MotorSpeedA , unsigned char MotorSpeedB)  {
  MotorSpeedA = map(MotorSpeedA, 0, 100, 0, 255);
  MotorSpeedB = map(MotorSpeedB, 0, 100, 0, 255);
  Wire.beginTransmission(I2CMotorDriverAdd); // transmit to device I2CMotorDriverAdd
  Wire.write(MotorSpeedSet);                 // set pwm header
  Wire.write(MotorSpeedA);                   // send pwma
  Wire.write(MotorSpeedB);                   // send pwmb
  Wire.endTransmission();                    // stop transmitting
}

void Motor(int speed) {
  if (speed > 0)MotorDirectionSet(0b1010);
  if (speed < 0)MotorDirectionSet(0b0101);
  unsigned char c = constrain(abs(speed), 0, 255);
  MotorSpeedSetAB(c, c);
  Serial.println(c);


}

void MotorPWMFrequenceSet(unsigned char Frequence)  {
  Wire.beginTransmission(I2CMotorDriverAdd); // transmit to device I2CMotorDriverAdd
  Wire.write(PWMFrequenceSet);               // set frequence header
  Wire.write(Frequence);                     //  send frequence
  Wire.write(Nothing);                       //  need to send this byte as the third byte(no meaning)
  Wire.endTransmission();                    // stop transmitting
}

void MotorDirectionSet(unsigned char Direction)  {     //  Adjust the direction of the motors 0b0000 I4 I3 I2 I1
  Wire.beginTransmission(I2CMotorDriverAdd);           // transmit to device I2CMotorDriverAdd
  Wire.write(DirectionSet);                            // Direction control header
  Wire.write(Direction);                               // send direction control information
  Wire.write(Nothing);                                 // need to send this byte as the third byte(no meaning)
  Wire.endTransmission();                              // stop transmitting
}

void MotorDriectionAndSpeedSet(unsigned char Direction, unsigned char MotorSpeedA, unsigned char MotorSpeedB)  { //you can adjust the direction and speed together
  MotorDirectionSet(Direction);
  MotorSpeedSetAB(MotorSpeedA, MotorSpeedB);
}

double heading(double x, double y) {
  double head = atan2(y, x); // Slope Y, Slope X
  return head;
}
void setup()  {
  Wire.begin(); // join i2c bus (address optional for master)
  delayMicroseconds(10000);
  accelemeter.init();
  Serial.begin(9600);
  Serial.println("setup begin");
}

void loop()  {
  float ax, ay, az;
  delay(10); //this delay needed
  accelemeter.getAcceleration(&ax, &ay, &az);
  float angle = heading(ax,az)/3.14*180.0-90;
  Serial.println(angle);
  Motor(angle*30);
  delay(100);


}
