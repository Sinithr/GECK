#include <Keypad.h>
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 
#include <SoftwareSerial.h>

#include "DHT.h"
#define DHTPIN 11    
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);    //czujnki w i t 
DHT dht2(10,DHTTYPE);

SoftwareSerial Genotronex(12, 13); // RX, TX bluetooth
int BluetoothData; 

//SoftwareSerial mySerial(0, 1); // RX, TX wifi
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27

#include "GECK.h"

///// Key
int i2caddress = 0x20;
const byte ROWS = 4;
const byte COLS = 3; 
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

/****************************************************************/





 GECK SYSTEM;
 
void setup() {
   pinMode(9, OUTPUT); // defaults HIGH (relays off)
   pinMode(A1,OUTPUT);
  digitalWrite(9,HIGH);
    digitalWrite(A1,HIGH);

  Genotronex.begin(9600);//Bluetooth
  Serial.begin(9600);
  Genotronex.println("Bluetooth On");
    delay(2000);

  lcd.begin(16,2);   
  lcd.noBacklight(); 
  //mySerial.begin(9600);
 // mySerial.println("Hello, world?");
}

void loop()
{ 
  SYSTEM.cycle();
   // Serial.write("serial");
   // mySerial.write("asd");
   
}

