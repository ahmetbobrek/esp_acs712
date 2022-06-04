#if !defined(ACS712_H)
#define ACS712_H
#include "Arduino.h"
enum scaleFactor {
  ACS712_5A = 185,
  ACS712_20A = 100,
  ACS712_30A = 66
};
class ACS712_Irms{
    private:
    //double ZeroSample = 0;
    double VoltsPerSample = 0;
    //double Voltage = 0;
    //double VRMS = 0;
    double AmpsRMS = 0;
    uint32_t start_time = 0;
    //int readValue = 0;
    int maxValue = 0;
    int minValue = 0;
    
  public:
    scaleFactor mVperAmp = scaleFactor::ACS712_20A; // use 100 for 20A Module and 66 for 30A Module and 185 for 5A Module

    int ADCIn = 0; //Analog input number
    double ADCSamples = 991.0; //1024 samples
    double maxADCVolt = 5.0; //5 Volts
    double ZeroCorrection = 0; //Calibration coefficient
    double VRMS = 0;
    double Vpp = 0;
    double Vpeak = 0;
    void Init();
    double Process();

};

#endif // ACS712_H
 