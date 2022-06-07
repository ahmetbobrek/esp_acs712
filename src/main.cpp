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
    float I = acs712.getCurrentDC(); 
    // Send it to serial
    Serial.println(String("I = ") + I + " A");
    Serial.println(analogRead(35));
    // Wait a second before the new measurement
    delay(300);
}
