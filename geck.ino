#include "GECK.h"

#include <Wire.h>
#include <Keypad.h> 
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>
#include <DHT.h>

//SoftwareSerial mySerial(0, 1); // RX, TX wifi

//int i2caddress = 0x20;

GECK system;

void setup() 
{
  system.configure();
}

void loop()
{ 
  system.cycle();
}


















