#include "arduino.h"
#include "acs712.h"
ACS712 acs712(ACS712_05B, 35);
int16_t cal;
 int16_t acc;

void setup()
{
    Serial.begin(9600);
    acs712.calibrate();
}
void loop()
{
    // We use 230V because it is the common standard in European countries
  // Change to your local, if necessary
  float U = 230;

  // To measure current we need to know the frequency of current
  // By default 50Hz is used, but you can specify desired frequency
  // as first argument to getCurrentAC() method, if necessary
  float I = acs712.getCurrentAC();

  // To calculate the power we need voltage multiplied by current
 
  float P = U * I;
 /*Serial.println(String("I = ") + I + " A");
  Serial.println(String("P = ") + P + " Watts");*/
  Serial.println(I);
  
  
 // Serial.println(acs712.shape());
}


