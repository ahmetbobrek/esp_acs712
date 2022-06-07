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
    float Irms=acs712.getCurrentAC();
    //float I = acs712.getCurrentDC(); 
    // Send it to serial
    //Serial.println(String("I = ") + I + " A");
    Serial.println(String("Irms = ") + Irms+ " A");
    Serial.println(analogRead(35));
    Serial.println(acs712.lsum);
    Serial.println(acs712.lnow);
     Serial.println(acs712.period);
     

    // Wait a second before the new measurement
    delay(300);
}
