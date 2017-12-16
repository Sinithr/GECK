#include "GECK.h"

GECK::GECK()
    : lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE),
      keys{{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}, {'#', '0', '*'}},
      rowPins{8, 7, 6, 5},
      colPins{4, 3, 2},
      keypad(makeKeymap(keys), rowPins, colPins, 4, 3),
      Genotronex(0, 1),  // RX, TX bluetooth
      lcd_state(LCD_STATE_SENSORS),
      turn_on_light(false),
      turn_on_water(false),
      turn_on_door(false),
      door_state(0),
      dht(DHTPIN, DHTTYPE),
      dht2(10, DHTTYPE) {}

void GECK::configure(void) {
  pinMode(9, OUTPUT);  // defaults HIGH (relays off)
  pinMode(A1, OUTPUT);
  digitalWrite(9, HIGH);
  digitalWrite(A1, HIGH);

  Genotronex.begin(9600);  // Bluetooth
  //Serial.begin(9600);
  Genotronex.println("Bluetooth On");
  delay(2000);

  lcd.begin(16, 2);
  //lcd.noBacklight();
}

void GECK::check_temperature_inside(void) {
  temperature_inside = dht.readTemperature();
  if (isnan(temperature_inside))
    temperature_inside = 0;
}

void GECK::check_humidity_soil(void) {
  humidity_soil = analogRead(A3);
}

void GECK::check_light_outside(void) {
  light_outside = analogRead(A2);
}

void GECK::check_humidity_temperature_outside(void) {
  temperature_outside = dht2.readTemperature();
  humidity_outside = dht2.readHumidity();
  if (isnan(dht2.readTemperature()) || isnan(dht2.readHumidity())) {
    temperature_outside = 0;
    humidity_outside = 0;
  }
}

void GECK::LCDA(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(" ");
  lcd.print(humidity_outside);
  lcd.print("%");

  lcd.setCursor(7, 0);
  lcd.print("T:");
  lcd.setCursor(9, 0);
  lcd.print(temperature_outside);
  lcd.print("|");

  lcd.print(temperature_inside);
  lcd.print(char(223));
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("G:");
  lcd.setCursor(3, 1);
  lcd.print(humidity_soil);

  lcd.setCursor(7, 1);
  lcd.print("L:");
  lcd.setCursor(10, 1);
  lcd.print(light_outside);
  lcd.print(" ");
}

void GECK::LCDB(void) {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("HELP");
  lcd.setCursor(3, 1);
  lcd.print("H-HUMIDITY");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T-TEMPERATURE");
  lcd.setCursor(0, 1);
  lcd.print("G-SOIL MOISTURE");

  delay(3000);

  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("L-SUNLIGHT ");
  lcd.setCursor(4, 1);
  lcd.print("EXPOSURE");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1/2-LIGHT OFF/ON ");
  lcd.setCursor(0, 1);
  lcd.print("3 - WATERING ");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("# - STATUS");

  delay(3000);
}

void GECK::LCDC(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("---STATUS---");
  lcd.setCursor(0, 1);
  lcd.print("SENSORS:");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EXTERNAL H&T: ");
  if (isnan(dht2.readTemperature()) || isnan(dht2.readHumidity()))
    lcd.print("E");
  else
    lcd.print("OK");

  lcd.setCursor(0, 1);
  lcd.print("MOISTURE: ");
  if (analogRead(A3) >= 900 || analogRead(A3) < 100)
    lcd.print("E");
  else
    lcd.print("OK");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SUNLIGHT: ");

  if (analogRead(A3) < 10)
    lcd.print("E");
  else
    lcd.print("OK");

  lcd.setCursor(0, 1);
  lcd.print("INTERNAL H&T: ");
  if (isnan(dht.readTemperature()))
    lcd.print("E");
  else
    lcd.print("OK");

  delay(3000);
}

void GECK::bluetooth(void) {
  if (Genotronex.available()) {
    BluetoothData = Genotronex.read();
    if (BluetoothData == '1') {
      Genotronex.print("Temperature inside (C): ");
      Genotronex.println(temperature_inside);
      Genotronex.print("Temperature outside (C): ");
      Genotronex.println(temperature_outside);
      Genotronex.print("Soil humidity: ");
      Genotronex.println(humidity_soil);
      Genotronex.print("Air humidity outside: ");
      Genotronex.println(humidity_outside);
      Genotronex.print("LUX: ");
      Genotronex.println(light_outside);
    }
  }
}

bool GECK::key(void) {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
    if (key == char(35))
      lcd_state = LCD_STATE_HELP;
    if (key == char(42))
      lcd_state = LCD_STATE_STATUS;
    if (key == '1')
      turn_on_light = turn_on_light ? false : true;
    if (key == '2')
      turn_on_water = true;
    if (key == '3')
      turn_on_door = true;
    return 1;
  }
  return 0;
}

void GECK::cycle(void) {
  unsigned long start;
  check_temperature_inside();
  check_humidity_soil();
  check_light_outside();
  check_humidity_temperature_outside();
  switch (lcd_state) {
    case LCD_STATE_SENSORS: {
      LCDA();
      break;
    }
    case LCD_STATE_HELP: {
      LCDB();
      lcd_state = LCD_STATE_SENSORS;
      break;
    }
    case LCD_STATE_STATUS: {
      LCDC();
      lcd_state = LCD_STATE_SENSORS;
      break;
    }
  }
  start = millis();
  do {
    if (turn_on_light == false)
      digitalWrite(9, HIGH);
    if (turn_on_light == true)
      digitalWrite(9, LOW);
    bluetooth();
    if (key()) {
      if (turn_on_light == false)
        digitalWrite(9, HIGH);
      if (turn_on_light == true)
        digitalWrite(9, LOW);
      if (turn_on_water == true) {
        turn_on_water = false;
        digitalWrite(A1, LOW);
        delay(3000);
        digitalWrite(A1, HIGH);
      }
      if (lcd_state != LCD_STATE_SENSORS)
        break;
    }
  } while ((millis() - start) <= 5000);
}

