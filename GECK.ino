
#include <SoftwareSerial.h>
#include "DHT.h"

#include <Keypad.h>
#include <Wire.h>   
#include <LiquidCrystal_I2C.h> 

#define DHTPIN 11    
#define DHTTYPE DHT11  

SoftwareSerial Genotronex(12, 13); // RX, TX bluetooth
DHT dht(DHTPIN, DHTTYPE);    //czujnki w i t 
DHT dht2(10,DHTTYPE);

//SoftwareSerial mySerial(0, 1); // RX, TX wifi
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Ustawienie adresu ukladu na 0x27


int BluetoothData; 

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
/////

class GECK
{
  int data[5];
  char X ='A';
  int light=-1;
  int water=-1;
  void Detector1(void)
  {
   // data[0]=dht.readHumidity();
    data[1]=dht.readTemperature();
    Serial.print("Wilgotnosc (%): ");   //wyświetlenie wartości wilgotności
    Serial.print(data[0]);
    Serial.print("\t\t");
    Serial.print("Temperatura (C): ");           //wyświetlenie temperatury
    Serial.println(data[1]);
  }
  
  void Detector2(void)
  {
    data[2] = analogRead(A3);      
    Serial.print("Moisture Sensor ");           
    Serial.println(analogRead(A3));
  }
  
  void Detector3(void)
  {
    data[3]= analogRead(A2);  
    Serial.print("LUX ");           
    Serial.println(analogRead(A2));
  }
  void Detector4(void)//W i T zew
  {
    
    data[4]=dht2.readTemperature();
    data[0]=dht2.readHumidity();
 
  }
  
  void LCDA(void)
  {
    
   lcd.clear();
   lcd.setCursor(0,0); 
   lcd.print("H:");
   lcd.print(" ");
   lcd.print(data[0]);
   lcd.print("%");
     
   lcd.setCursor(7,0); 
   lcd.print("T:");
   lcd.setCursor(9,0);
   lcd.print(data[4]);
   lcd.print("|");
   lcd.print(data[1]);
   lcd.print(char(223));
   lcd.print("C");
   
   lcd.setCursor(0,1); 
   lcd.print("G:");
   lcd.setCursor(3,1);
   lcd.print(data[2]);   
  
  
   lcd.setCursor(7,1); 
   lcd.print("L:");
   lcd.setCursor(10,1);
   lcd.print(data[3]);   
   lcd.print(" ");
   
  
  
  }
  
  void LCDB(void)
  {
    
   lcd.clear();
   lcd.setCursor(6,0); 
   lcd.print("HELP");
   
   lcd.setCursor(3,1); 
   lcd.print("H-HUMIDITY");
   delay(2000);
   
   lcd.clear();
   lcd.setCursor(0,0); 
   lcd.print("T-TEMPERATURE");
   
   lcd.setCursor(0,1); 
   lcd.print("G-SOIL MOISTURE");
   delay(2000);
   
   lcd.clear();
   lcd.setCursor(3,0); 
   lcd.print("L-SUNLIGHT ");
    lcd.setCursor(4,1); 
   lcd.print("EXPOSURE");
   delay(2000);
  
  }
  
  
  void bluetooth(void)
  {
    if (Genotronex.available() ) 
   {
         BluetoothData=Genotronex.read();
         if(BluetoothData=='1')
             {  
                  Genotronex.print("Temperatura (C):");
                  Genotronex.println(data[1]);
                  Genotronex.print("Wilgotnosc (%):");
                  Genotronex.println(data[0]);
                  Genotronex.print("Wilgotnosc glaby");
                  Genotronex.println(data[2]);
                  Genotronex.print("LUX");
                  Genotronex.println(data[3]);
             }
      
   }
  }

  void key()
  {
     char key = kpd.getKey();

     if (key != NO_KEY){
        Serial.println(key);
        if(key==char(35)) X='B';
        if(key=='1')  light=0 ;
        if(key=='2')  light=1 ;
        if(key=='4')  water=0 ;
        if(key=='5')  water=1 ;
            
        }
        
        
        
       
  
  }
  public:
  void cycle(void)
  {
     
     switch(X)
     {
       case 'A':
       {
         unsigned long start=millis();
         Detector1();
         Detector2();
         Detector3();
         Detector4();
         LCDA();
         
         if (light==0)
         {
             light=-1;
             digitalWrite (9, HIGH);
             delay(1000); 
         }   
         if (light==1)
         {
             light=-1;
             digitalWrite (9, LOW);
             delay(1000);
         }
         if (water==0)
         {
             water=-1;
             digitalWrite (A1, HIGH);
             delay(1000); 
         }   
         if (water==1)
         {
             water=-1;
             digitalWrite (A1, LOW);
             delay(1000);
         }
         
         do{
         bluetooth();
         key();
         delay(500);
         }
         while ((millis()-start)<=5000);
         break;
       }
       case 'B':
       {
         LCDB();    
         X='A';
         break;
       }
     
     
     }  
}

};


 GECK SYSTEM;
 
void setup() {
   pinMode(9, OUTPUT); // defaults HIGH (relays off)
   pinMode(A1,OUTPUT);
  digitalWrite(9,HIGH);
    digitalWrite(A1,HIGH);

  Genotronex.begin(9600);//Bluetooth
  Serial.begin(9600);
  Genotronex.println("Bluetooth On");
    delay(3000);

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

