#include <Arduino.h>
#include <Print.h>
#include "acs712.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include"RTClib.h"
/*RTC_DS1307 rtc;*/
RTC_DS1307 rtc;
/*char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};*/
char daysOfTheWeek[7][12]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



RTC_DATA_ATTR byte bootCount = 0;
RTC_DATA_ATTR time_t now;
RTC_DATA_ATTR uint64_t Mics = 0;
RTC_DATA_ATTR struct tm *timeinfo;
#define OFFSET 0
#define sleepPeriod 10000000ul
String dosya;
ACS712 acs712_1(ACS712_05B, 4);
ACS712 acs712_2(ACS712_05B, 25);
ACS712 acs712_3(ACS712_05B, 15);
int16_t cal1;
int16_t cal2;
int16_t cal3;
int16_t acc;
String sutun1 = "Örnek_Sayısı";
String sutun2 = "AC_akım1";
String sutun3 = "DC akım1";
String sutun4 = "AC_akım2";
String sutun5 = "DC akım2";
String sutun6 = "AC_akım3";
String sutun7 = "DC_akım3";
String str = sutun1 + "," + sutun2 + "," + sutun3 + "," + sutun4 + "," + sutun5 + "," + sutun6 + "," + sutun7+"\n\n";


///////////////////////////////SD KART FONKSİYONLARI////////////////////////

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
void createDir(fs::FS &fs, const char *path)
{

  Serial.printf("Creating Dir:%s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("Mkdir failed");
  }
}
void removeDir(fs::FS &fs, const char *path)
{
  Serial.printf("Removing Dir:%s\n", path);
  if (fs.rmdir(path))
  {
    Serial.printf("Dir removed");
  }
  else
  {
    Serial.println("rmdir failed");
  }
}
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for wwriting");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}
void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Appended failed");
    return;
  }
  file.close();
}
////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(115200);
  /// ACS
  cal1 = acs712_1.calibrate();
  cal2 = acs712_2.calibrate();
  cal3 = acs712_3.calibrate();
  ////////RTC
  /*if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }*/
  if (!rtc.begin())
  {
    Serial.println("Couldnt find RTC");
    Serial.flush();
    while(1) delay(10);
  }
  /*if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }*/
  if (!rtc.isrunning())
  {
    Serial.println("RTC is not running");
    rtc.adjust(DateTime(2014,1,21,3,0,0));
  }
  

  ////SD
  if (!SD.begin())
  {
    Serial.println("Card mount failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  createDir(SD, "/data");

  writeFile(SD, "/akim_data.txt", str.c_str());
  appendFile(SD, "/hello.txt", "World!\n");

}

void loop()
{
 
    DateTime now=rtc.now();
    /*  Serial.println(now.year(),DEC);
    Serial.println(daysOfTheWeek[now.dayOfTheWeek()]);
    DateTime future (now + TimeSpan(7,12,30,6));*/
  String olcum="1,"+String(acs712_1.getCurrentAC())+","+String(acs712_1.getCurrentDC())+","+String(acs712_2.getCurrentAC())+","+String(acs712_2.getCurrentDC())
  +","+String(acs712_3.getCurrentAC())+","+String(acs712_3.getCurrentDC())+"\n";
  
  appendFile(SD, "/akim_data.txt", olcum.c_str());
  Serial.println(olcum);

  delay(300);

}
