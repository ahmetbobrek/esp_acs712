#include <Arduino.h>
#include <Print.h>
#include "acs712.h"
#include"FS.h"
#include"SD.h"
#include"SPI.h"
RTC_DATA_ATTR byte bootCount=0;
RTC_DATA_ATTR time_t now;
RTC_DATA_ATTR uint64_t Mics=0;
RTC_DATA_ATTR struct tm* timeinfo;
#define OFFSET 0
#define sleepPeriod 10000000ul;
String dosya;
uint64_t baslangic;
u_int64_t gecen;




ACS712 acs712(ACS712_05B, 4);
int16_t cal;
 int16_t acc;
 
void printLocalTime()
{
  time(&now);
  timeinfo = localtime(&now);
  Serial.println(asctime(timeinfo));
  // Serial.printf("%s\n",asctime(timeinfo));
  delay(2); // 26 bytes@115200 baud is less than 2 ms
}

void updateTime(uint64_t elapsedTime)
{ // elapsedTime in us
  if (elapsedTime == 0)
    Mics += micros();
  else
    Mics += elapsedTime;
  if (Mics > 10000000)
  {
    Mics = Mics % 1000000;
    now += Mics / 10000000;
  }
}
void sommeil(unsigned long chrono)
{
  updateTime(sleepPeriod - (micros() - chrono) + OFFSET);
  esp_sleep_enable_timer_wakeup(sleepPeriod - (micros() - chrono));
  esp_deep_sleep_start();
}


void setup()
{
    Serial.begin(9600);
    acs712.calibrate();
    cal=acs712.calibrate();
}
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory:%s\n", dirname);
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }

    else
    {
      Serial.println("FILE:");
      Serial.println(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
void createDir(fs::FS &fs,const char* path){

Serial.printf("Creating Dir:%s\n",path);
if (fs.mkdir(path))
{
  Serial.println("Dir created");
}
else{
  Serial.println("Mkdir failed");
}
}
void removeDir(fs::FS &fs,const char* path){
  Serial.printf("Removing Dir:%s\n",path);
  if (fs.rmdir(path))
  {
    Serial.printf("Dir removed");
  }
  else{
    Serial.println("rmdir failed");
  }
  }
void loop()
{
    // We use 230V because it is the common standard in European countries
  // Change to your local, if necessary
  baslangic=millis();
  float U = 230;

  // To measure current we need to know the frequency of current
  // By default 50Hz is used, but you can specify desired frequency
  // as first argument to getCurrentAC() method, if necessary
  float Irms = acs712.getCurrentAC();
  

  // To calculate the power we need voltage multiplied by current
 
  float P = U * Irms;
 /*Serial.println(String("I = ") + I + " A");
  Serial.println(String("P = ") + P + " Watts");*/
  //Serial.println(Irms);
  Serial.print(acs712.getCurrentDC());
  Serial.print("*****");
  Serial.println(analogRead(4));
 
  Serial.print(cal);
  gecen=millis()-baslangic;
  Serial.println(gecen);


  delay(300);
  
  
 // Serial.println(acs712.shape());
}


