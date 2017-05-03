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
    
    void Detector1(void);     
    void Detector2(void);   
    void Detector3(void);
    void Detector4(void);
 
    void LCDA(void) //LCD data display
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
  
    void LCDB(void)  //HELP
    { 
        lcd.clear();
        lcd.setCursor(6,0); 
        lcd.print("HELP");
        lcd.setCursor(3,1); 
        lcd.print("H-HUMIDITY");
  
        delay(3000);
   
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("T-TEMPERATURE");
        lcd.setCursor(0,1); 
        lcd.print("G-SOIL MOISTURE");
        
        delay(3000);
   
        lcd.clear();
        lcd.setCursor(3,0); 
        lcd.print("L-SUNLIGHT ");
        lcd.setCursor(4,1); 
        lcd.print("EXPOSURE");
        
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("1/2-LIGHT OFF/ON ");
        lcd.setCursor(0,1); 
        lcd.print("3 - WATERING ");
        
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0,0);  
        lcd.print("# - STATUS");
        
        delay(3000);
        
    }
    void LCDC(void) //STATUS
    {
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("---STATUS---");
        lcd.setCursor(0,1); 
        lcd.print("SENSORS:"); 
        
        delay(3000);
        
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("EXTERNAL H&T: ");
        if(isnan(dht2.readTemperature() )|| isnan(dht2.readHumidity() )) lcd.print("E");
        else lcd.print("OK"); 
        
        
        lcd.setCursor(0,1); 
        lcd.print("MOISTURE: ");
        if(analogRead(A3) >=900 || analogRead(A3)<100 ) lcd.print("E");
        else lcd.print("OK");
        
        delay(3000);
   
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("SUNLIGHT: ");
        if (analogRead(A3)<10) lcd.print("E");
        else lcd.print("OK"); 
        lcd.setCursor(0,1); 
        lcd.print("INTERNAL H&T: ");
        if (isnan(dht.readTemperature() )) lcd.print("E");
        else lcd.print("OK"); 
        
        delay(3000);
        
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

    bool key()
    {
        char key = kpd.getKey();
        if (key != NO_KEY)
        {
            Serial.println(key);
            if(key==char(35)) X='B';
            if(key==char(42)) X='C';
            if(key=='1')  light=0 ;
            if(key=='2')  light=1 ;
            if(key=='3')  water=1 ;
            return 1;
        }
        return 0;  
        
        
        
       
  
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
         
            do
            {
               if (light==0)
                    {                       
                        digitalWrite (9, HIGH);
                    }   
               if (light==1)
                    {
                        digitalWrite (9, LOW);
                    }
                bluetooth();
                if (key())
                {
                    if (light==0)
                    {                       
                        digitalWrite (9, HIGH);
                    }   
                    if (light==1)
                    {
                        digitalWrite (9, LOW);
                    }
                    if (water==1)
                    {
                        water=-1;
                        digitalWrite (A1, LOW);
                        delay(3000);
                        digitalWrite (A1, HIGH);
                    }
                    if(X!='A')break;
         
                }
         
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
        case 'C':
        {
            LCDC();
            X='A';
            break;
        }
        
        
        }  
    }

};

#endif
