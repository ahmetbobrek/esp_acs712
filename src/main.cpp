#include "acs712.h"
const int sensorIn = A0;
ACS712_Irms acs712;

void setup()
{
    Serial.begin(9600);
    acs712.ADCSamples = 1024.0;                // 1024 samples
    acs712.mVperAmp = scaleFactor::ACS712_20A; // use 100 for 20A Module and 66 for 30A Module and 185 for 5A Module
    acs712.maxADCVolt = 5.0;                   // 5 Volts
    acs712.ADCIn = A0;
    acs712.Init();
}

void loop()
{
    double AmpsRMS = acs712.Process();
    // Serial.println("Amps RMS: " + String(AmpsRMS));
}
