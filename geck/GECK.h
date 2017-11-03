#ifndef GECK_H
#define GECK_H

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <DHT.h>
#define DHTPIN 11
#define DHTTYPE DHT11

class GECK {
 public:
  void cycle(void);
  void configure(void);
  GECK(void);

 private:
  enum LCD_STATES { LCD_STATE_SENSORS, LCD_STATE_HELP, LCD_STATE_STATUS };
  LCD_STATES lcd_state;
  bool turn_on_light, turn_on_water, lcd_background_light;
  int light_outside, humidity_outside, humidity_soil, temperature_inside,
      temperature_outside;
  const char keys[4][3];
  byte rowPins[4];  // connect to the row pinouts of the keypad
  byte colPins[3];  // connect to the column pinouts of the keypad
  LiquidCrystal_I2C lcd;
  Keypad keypad;
  SoftwareSerial Genotronex;
  int BluetoothData;
  DHT dht, dht2;

  void check_temperature_inside(void);  // H&T internal
  void check_humidity_soil(void);  // Moisture
  void check_light_outside(void);  // LUX
  void check_humidity_temperature_outside(void);
  void LCDA(void);  // LCD data display
  void LCDB(void);  // HELP
  void LCDC(void);  // STATUS
  void bluetooth(void);
  bool key(void);
};

#endif

