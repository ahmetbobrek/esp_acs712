#include "acs712.h"
void ACS712_Irms::Init()
    {
      //ZeroSample = ADCSamples/2;
      VoltsPerSample = maxADCVolt/ADCSamples;
      minValue = ADCSamples;
    }
double ACS712_Irms::Process()
    {
        if (millis() - start_time < 250)
      {
        int readValue = analogRead(ADCIn);
        maxValue = (readValue > maxValue) ? readValue : maxValue;
        minValue = (readValue < minValue) ? readValue : minValue;
      }
      else
      {
        Vpp = (maxValue - minValue) * VoltsPerSample; //Volts Peak to Peak
        Vpeak = Vpp/2; //Volts peak/Amplitude
        VRMS = Vpeak * 0.707;
        AmpsRMS = (VRMS * 1000)/mVperAmp - ZeroCorrection;
        Serial.println("Amps RMS: " + String(AmpsRMS));
        maxValue = 0;
        minValue = ADCSamples;
        start_time = millis();
      } 
      return AmpsRMS;

    }    