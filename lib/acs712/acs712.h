#if !defined(ACS712_H)
#define ACS712_H
#include <Arduino.h>

#define ADC_SCALE 4095
#define VREF 3.3
enum ACS712_type {
  ACS712_05B,ACS712_20A,ACS712_30A
};

class ACS712{
  public:
    ACS712(ACS712_type type,uint8_t _pin);
    int calibrate();
    void setZeroPoint(int _zero);
    float getCurrentDC();
    float getCurrentAC(uint16_t frequency = 50);
    uint32_t lsum=0,measurements_count=0;int32_t lnow; uint32_t period; uint32_t t_start;
   
    

    

    

    /*scaleFactor mVperAmp = scaleFactor::ACS712_20A; // use 100 for 20A Module and 66 for 30A Module and 185 for 5A Module

    int ADCIn = 0; //Analog input number
    double ADCSamples = 991.0; //1024 samples
    double maxADCVolt = 5.0; //5 Volts
    double ZeroCorrection = 0; //Calibration coefficient
    double VRMS = 0;
    double Vpp = 0;
    double Vpeak = 0;
    void Init();
    double Process();*/
    


  private:
  /* //double ZeroSample = 0;
  double VoltsPerSample = 0;
  //double Voltage = 0;
  //double VRMS = 0;
  double AmpsRMS = 0;
  uint32_t start_time = 0;
  //int readValue = 0;
  int maxValue = 0;
  int minValue = 0;  */
  float sensitivity;
  uint8_t pin;
  int16_t zero;  
  
};

#endif // ACS712_H
 