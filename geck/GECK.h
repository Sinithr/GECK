#ifndef GECK_H
#define GECK_H

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <DHT.h>
#include <Servo.h>
#include <PID_v1.h>
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
  bool turn_on_light, turn_on_water, lcd_background_light,
       turn_on_door, pid_on, auto_watering;
  int light_outside, humidity_outside, humidity_soil, temperature_inside,
      temperature_outside, door_state, watering_threshold;
  const char keys[4][3];
  byte rowPins[4];  // connect to the row pinouts of the keypad
  byte colPins[3];  // connect to the column pinouts of the keypad
  LiquidCrystal_I2C lcd;
  Keypad keypad;
  SoftwareSerial Genotronex;
  int BluetoothData;
  DHT dht, dht2;
  Servo servo;
  double pid_setpoint, pid_input, pid_output;
  PID pid_controller;

  void check_temperature_inside(void);  // H&T internal
  void check_humidity_soil(void);  // Moisture
  void check_light_outside(void);  // LUX
  void check_humidity_temperature_outside(void);
  void LCDA(void);  // LCD data display
  void LCDB(void);  // HELP
  void LCDC(void);  // STATUS
  void set_servo_position(int);
  void servo_move(int);
  void turn_pid(bool);
  bool bluetooth(void);
  bool key(void);
};

#endif

