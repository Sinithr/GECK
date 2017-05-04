#include "GECK.h"

void GECK::Detector1(void) 
{  
  data[1]=dht.readTemperature(); 
  if (isnan(data[1])) 
    data[1]=0;
}       

void GECK::Detector2(void) 
{
  data[2] = analogRead(A3);
}      

void GECK::Detector3(void) 
{
  data[3]= analogRead(A2); 
}  

void GECK::Detector4(void) 
{
  data[4]=dht2.readTemperature();
  data[0]=dht2.readHumidity();
  if (isnan(dht2.readTemperature()) || 
    isnan(dht2.readHumidity()))
  { 
    data[4]=0;
    data[0]=0;
  } 
}

void GECK::LCDA(void) 
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

void GECK::LCDB(void) { 
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

void GECK::LCDC(void)
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
  if (isnan(dht2.readTemperature()) || 
    isnan(dht2.readHumidity())) 
    lcd.print("E");
  else 
    lcd.print("OK"); 


  lcd.setCursor(0,1); 
  lcd.print("MOISTURE: ");
  if (analogRead(A3) >=900 || 
    analogRead(A3)<100 ) 
    lcd.print("E");
  else 
    lcd.print("OK");

  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0); 
  lcd.print("SUNLIGHT: ");

  if (analogRead(A3)<10) 
    lcd.print("E");
  else 
    lcd.print("OK"); 

  lcd.setCursor(0,1); 
  lcd.print("INTERNAL H&T: ");
  if (isnan(dht.readTemperature())) 
    lcd.print("E");
  else lcd.print("OK"); 

  delay(3000);
}


void GECK::bluetooth(void)
{
  if (Genotronex.available()) 
  {
    BluetoothData=Genotronex.read();
    if (BluetoothData=='1')
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

bool GECK::key()
{
  char key = kpd.getKey();
  if (key != NO_KEY)
  {
    Serial.println(key);
    if (key==char(35)) X='B';
    if (key==char(42)) X='C';
    if (key=='1')  light=0 ;
    if (key=='2')  light=1 ;
    if (key=='3')  water=1 ;
    return 1;
  }
  return 0;  
}

void GECK::cycle(void)
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
          digitalWrite (9, HIGH);   
        if (light==1)
          digitalWrite (9, LOW);
        bluetooth();
        if (key())
        {
          if (light==0)
            digitalWrite (9, HIGH);
          if (light==1)
            digitalWrite (9, LOW);
          if (water==1)
          {
            water=-1;
            digitalWrite (A1, LOW);
            delay(3000);
            digitalWrite (A1, HIGH);
          }
          if (X!='A')
            break;
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





