#include "GECK.h"
#include <Wire.h>
#include <Keypad.h> 
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>
#include <DHT.h>
#include <Servo.h>
#include <PID_v1.h>

GECK system;

void setup() 
{
  system.configure();
}

void loop()
{ 
  system.cycle();
}
