#if !defined(ACS712_H)
#define ACS712_H
#include <Arduino.h>

#define ADC_SCALE 4095
#define VREF 3.3
#define measure_const 0.004356004
enum ACS712_type {
  ACS712_05B,ACS712_20A,ACS712_30A
};

class ACS712{
  public:
  ACS712(ACS712_type type, uint8_t _pin);
	int calibrate();
	void setZeroPoint(int _zero);
	void setSensitivity(float sens);
	float getCurrentDC();
	float getCurrentAC(uint16_t frequency = 50);
  int16_t shape();
  int16_t acc;

    
   
    

    

    

    


  private:
 
  float sensitivity;
  uint8_t pin;
  int16_t zero;  
  
};

#endif // ACS712_H
 