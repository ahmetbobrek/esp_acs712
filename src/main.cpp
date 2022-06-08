#include "arduino.h"
#include "acs712.h"
ACS712 acs712(ACS712_05B, 35);
int16_t cal;


void setup()
{
    Serial.begin(9600);
    Serial.println("Calibrating... Ensure that no current flows through the sensor at this moment");
    int zero = acs712.calibrate(); 
 
}
void loop()
{
    //float Irms=acs712.getCurrentAC();
    //float I = acs712.getCurrentDC(); 
    // Send it to serial
    //Serial.println(String("I = ") + I + " A");
    //Serial.println(String("Irms = ") + acs712.Irms+ " A");
    Serial.println(analogRead(35));
    /*float ACS712::getCurrentAC(uint16_t frequency){
  period=1000000/frequency;
  t_start=micros(); 

  
  while (micros()-t_start<period)
  {
      lnow=analogRead(35)-zero;
      lsum+=lnow*lnow;
      measurements_count++;    
      
  }
  float Irms=sqrt(lsum/measurements_count)/ADC_SCALE*VREF/sensitivity;
  return lsum;
}*/
acs712.t_start=micros();
for (int i = 0; i < 50; i++)
{
   acs712.lnow=analogRead(35)-2560;
    acs712.lsum+=sq(acs712.lnow);
    acs712.measurements_count++;
    }
    Serial.println(acs712.measurements_count);
    Serial.println(acs712.lnow);
    Serial.println(sq(acs712.lnow));
    /*Serial.println(acs712.lsum);
    Serial.println(sqrt(acs712.lsum)/acs712.measurements_count);*/
    Serial.println(acs712.lsum);
    Serial.println(sqrt(acs712.lsum/acs712.measurements_count)*3.3/4096/0.185);
    Serial.println("zaman:"+String(micros()-acs712.t_start));
     
    Serial.println("************************");
    acs712.lsum=0;
    acs712.measurements_count=0;
    delay(300);
}


 /*while (micros()-acs712.t_start<20000)
{
    
     

    // Wait a second before the new measurement
    delay(300);
}*/

