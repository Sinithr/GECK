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
      pid_on(true),
      auto_watering(true),
      auto_light(true),
      watering_threshold(400),
      door_state(0),
      dht(DHTPIN, DHTTYPE),
      dht2(10, DHTTYPE),
      pid_setpoint(22),
      pid_controller( &pid_input, 
                      &pid_output,
                      &pid_setpoint, 
                      6, 
                      10, 
                      1, 
                      REVERSE) {}

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
  servo.attach(13);
  pid_controller.SetMode(AUTOMATIC);
  pid_controller.SetOutputLimits(0, 100);
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
  lcd.setCursor(2, 1);
  lcd.print(humidity_soil);

  lcd.setCursor(6, 1);
  lcd.print("L:");
  lcd.setCursor(8, 1);
  lcd.print(light_outside);
  lcd.setCursor(11, 1);
  lcd.print(auto_light ? "Y" : "N");
  lcd.setCursor(12, 1);
  lcd.print(auto_watering ? "Y" : "N");
  lcd.setCursor(13, 1);
  lcd.print(pid_on ? "Y" : "N");
  lcd.setCursor(14, 1);
  lcd.print(pid_setpoint);
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
  lcd.setCursor(4, 0);
  lcd.print("1st Y/N");
  lcd.setCursor(3, 1);
  lcd.print("AUTO LIGHT");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("2nd Y/N");
  lcd.setCursor(2, 1);
  lcd.print("AUTO WATERING");

  delay(3000);

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("3rd Y/N");
  lcd.setCursor(6, 1);
  lcd.print("PID");
  
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("LAST NUMBERS");
  lcd.setCursor(2, 1);
  lcd.print("PID SETPOINT");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1-LIGHT OFF/ON ");
  lcd.setCursor(0, 1);
  lcd.print("2-WATERING ");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("3-OPEN/CLOSE");
  lcd.setCursor(0, 1);
  lcd.print("DOOR");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("4-DECREASE");
  lcd.setCursor(0, 1);
  lcd.print("PID SETPOINT");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("5-INCREASE");
  lcd.setCursor(0, 1);
  lcd.print("PID SETPOINT");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("6-PID ON/OFF");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("7-DECREASE");
  lcd.setCursor(0, 1);
  lcd.print("WATERING THRESH.");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("8-INCREASE");
  lcd.setCursor(0, 1);
  lcd.print("WATERING THRES.");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("9-AUTO WATERING");
  lcd.setCursor(0, 1);
  lcd.print("ON/OFF");

  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("0-AUTO LIGHT");
  lcd.setCursor(0, 1);
  lcd.print("ON/OFF");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("# - STATUS");

  delay(3000);
}

void GECK::LCDC(void) {
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

  delay(3000); //pid_on
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PID state: ");
  if(pid_on)
    lcd.print("ON");
  else
    lcd.print("OFF");
  lcd.setCursor(0, 1);
  lcd.print("PID setpoint: ");
  lcd.print(pid_setpoint);
  lcd.print("C");
  
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Auto watering");
  lcd.setCursor(0, 1);
  lcd.print("state: ");
  if(auto_watering)
    lcd.print("ON");
  else
    lcd.print("OFF");
    
  delay(3000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Watering");
  lcd.setCursor(0, 1);
  lcd.print("threshold: ");
  lcd.print(watering_threshold);
  
  delay(3000);
}

bool GECK::bluetooth(void) {
  if(Genotronex.available()) {
    BluetoothData = Genotronex.read();
    if (BluetoothData == '1') {
      turn_on_light = turn_on_light ? false : true;
      auto_light = false;
    }
    else if(BluetoothData == '2')
      turn_on_water = true;
    else if(BluetoothData == '3') {
      turn_on_door = true;
      pid_on = false;
      turn_pid(pid_on);
    }
    else if(BluetoothData == '4')
      pid_setpoint--;
    else if(BluetoothData == '5')
      pid_setpoint++;
    else if(BluetoothData == '6') {
      pid_on = pid_on ? false : true;
      turn_pid(pid_on);
    }
    else if(BluetoothData == '7')
      watering_threshold -= 10;
    else if(BluetoothData == '8')
      watering_threshold += 10;
    else if(BluetoothData == '9') {
      auto_watering = auto_watering ? false : true;
    }
    else if(BluetoothData == '0') {
      auto_light = auto_light ? false : true;
    }
    else if(BluetoothData == 'd' || BluetoothData == 'D') {
      servo.detach(); // because of overwritting
      Genotronex.print("Temperature inside (C): ");
      Genotronex.println(temperature_inside);
      Genotronex.print("Temperature outside (C): ");
      Genotronex.println(temperature_outside);
      Genotronex.print("Soil humidity: ");
      Genotronex.println(humidity_soil);
      Genotronex.print("Air humidity outside(%): ");
      Genotronex.println(humidity_outside);
      Genotronex.print("LUX: ");
      Genotronex.println(light_outside);
      Genotronex.print("PID state: ");
      Genotronex.println(pid_on ? "ON" : "OFF");
      Genotronex.print("PID setpoint: ");
      Genotronex.println(pid_setpoint);
      Genotronex.print("Auto watering state: ");
      Genotronex.println(auto_watering ? "ON" : "OFF");
      Genotronex.print("watering threshold: ");
      Genotronex.println(watering_threshold);
      Genotronex.print("Auto light: ");
      Genotronex.println(auto_light ? "ON" : "OFF");
      servo.attach(13); // turning on
    } else
      return 0;
    return 1;
  }
  return 0;
}

bool GECK::key(void) {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.println(key);
    if (key == char(35))
      lcd_state = LCD_STATE_HELP;
    if (key == char(42))
      lcd_state = LCD_STATE_STATUS;
    if (key == '1') {
      turn_on_light = turn_on_light ? false : true;
      auto_light = false;
    }
    if (key == '2')
      turn_on_water = true;
    if (key == '3') {
      turn_on_door = true;
      pid_on = false;
      turn_pid(pid_on);
    }
    if(key == '4')
      pid_setpoint--;
    if(key == '5')
      pid_setpoint++;
    if(key == '6') {
      pid_on = pid_on ? false : true;
      turn_pid(pid_on);
    }
    if(key == '7')
      watering_threshold -= 10;
    if(key == '8')
      watering_threshold += 10;
    if(key == '9') {
      auto_watering = auto_watering ? false : true;
    }
    if(key == '0') {
      auto_light = auto_light ? false : true;
    }
    return 1;
  }
  return 0;
}

void GECK::cycle(void) {
  static int counter = 0;
  unsigned long start;
  check_temperature_inside();
  check_humidity_soil();
  check_light_outside();
  check_humidity_temperature_outside();
  if(auto_light) {
      if(light_outside < 100)
        turn_on_light = true;
      else
        turn_on_light = false;
    }
  pid_input = temperature_inside;
  if(counter++ >= 12) {
    if(auto_watering)
      if(humidity_soil >= watering_threshold)
        turn_on_water = true;
    if(pid_on) {
      pid_controller.Compute();
      set_servo_position(pid_output);
    }
    counter = 0;
  }
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
    if (key() || bluetooth()) {
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
      if (turn_on_door == true) {
         turn_on_door = false;
         if (door_state == 0) {
           set_servo_position(100);
         } else {
           set_servo_position(0);
         }
         
      }
      if (lcd_state != LCD_STATE_SENSORS)
        break;
    }
  } while ((millis() - start) <= 5000);
}

void GECK::set_servo_position(int pos) {
  if(abs(pos-door_state) < 1)
    return;
  if(abs(pos-door_state) < 5) // very small change could be inaccurate
    if(pos == 0)
      set_servo_position(15);
    else if(pos == 100)
      set_servo_position(85);
    else
      return;
  int move_direction;
  if(pos > door_state)
    move_direction = 130;
  else
    move_direction = 50;
  servo_move(move_direction);
  delay(abs(pos-door_state) * 20);
  servo_move(90);
  door_state = pos;
}

void GECK::servo_move(int val) {
   servo.write(val); 
}

void GECK::turn_pid(bool on_off) {
    if(on_off)
      pid_controller.SetMode(AUTOMATIC);
    else
      pid_controller.SetMode(MANUAL);
}
