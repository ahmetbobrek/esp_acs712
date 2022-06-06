#include "acs712.h"
ACS712::ACS712(ACS712_type type,uint8_t _pin){
pin=_pin;
  switch(type){
    case ACS712_05B:
      sensitivity=0.185;
      break;
    case ACS712_20A:
      sensitivity=0.100;
      break;
    case ACS712_30A:
      sensitivity=0.066;
      break;  
  }
}
int ACS712::calibrate(){
  uint16_t acc=0;
  for (int i = 0; i < 10; i++)
  {
    acc+=analogRead(pin);
    zero=acc/10;
    return zero;
  }
}
void ACS712::setZeroPoint(int _zero){
  zero=_zero;
}
float ACS712::getCurrentDC(){
  int16_t acc=0;
  for (int i = 0; i < 10; i++)
  {
    acc+=analogRead(pin)-zero;
  }
  float I=(float)acc/10.0/ADC_SCALE*VREF/sensitivity;
  return I;
  
}
float ACS712::getCurrentAC(uint16_t frequency){
  uint32_t period=1000000/frequency;
  uint32_t t_start=micros();
  uint32_t lsum=0,measurements_count=0;
  int32_t lnow;
  while (micros()-t_start<period)
  {
      lnow=analogRead(pin)-zero;
      lsum+=lnow*lnow;
      measurements_count++;
    float Irms=sqrt(lsum/measurements_count)/ADC_SCALE*VREF/sensitivity;
    return Irms;
  }
  
}

/*void ACS712_Irms::Init()
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

    }    */

