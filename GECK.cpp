#include "GECK.h"

void GECK::Detector1(void) { //H&T internal 
  data[1]=dht.readTemperature(); 
  if(isnan(data[1])) data[1]=0;
}       

void GECK::Detector2(void) {
  data[2] = analogRead(A3);
} //Moisture     

void GECK::Detector3(void) { //LUX
  data[3]= analogRead(A2); 
}  

void GECK::Detector4(void) 
{
  data[4]=dht2.readTemperature();
  data[0]=dht2.readHumidity();
  if(isnan(dht2.readTemperature())|| isnan(dht2.readHumidity() ))
  { 
    data[4]=0;
    data[0]=0;


  } 
}

