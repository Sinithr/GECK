#ifndef GECK_H
#define GECK_H

#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>
#include <Keypad.h>
#include "DHT.h"
extern LiquidCrystal_I2C lcd;
extern SoftwareSerial Genotronex;
extern Keypad kpd;
extern DHT dht;
extern DHT dht2;
extern int BluetoothData;

class GECK
{
private:
  int data[5];
  char X ='A';
  int light=-1;
  int water=-1;

  void Detector1(void); // H&T internal   
  void Detector2(void); //Moisture
  void Detector3(void); // LUX
  void Detector4(void);
  void LCDA(void); // LCD data display
  void LCDB(void); // HELP
  void LCDC(void); // STATUS
  void bluetooth(void);
  bool key();
public:
  void cycle(void);
};

#endif


