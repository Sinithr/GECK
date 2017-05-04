#ifndef GECK_H
#define GECK_H

#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <DHT.h>
#define DHTPIN 11
#define DHTTYPE DHT11


class GECK
{
public:
  void cycle(void);
  GECK(void);
  
private:
  int data[5];
  char X;
  int light;
  int water;
  const char keys[4][3];
  byte rowPins[4]; //connect to the row pinouts of the keypad
  byte colPins[3]; //connect to the column pinouts of the keypad
  LiquidCrystal_I2C lcd;
  Keypad kpd;
  SoftwareSerial Genotronex;
  int BluetoothData; 
  DHT dht;//czujnki w i t 
  DHT dht2;
  
  void Detector1(void); // H&T internal   
  void Detector2(void); //Moisture
  void Detector3(void); // LUX
  void Detector4(void);
  void LCDA(void); // LCD data display
  void LCDB(void); // HELP
  void LCDC(void); // STATUS
  void bluetooth(void);
  bool key(void);
};

#endif


