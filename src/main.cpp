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
#define sleepPeriod 10000000ul
String dosya;



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

void writeFile(fs::FS &fs,const char* path,const char* message){
  Serial.printf("Writing file: %s\n",path);
  File file=fs.open(path,FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for wwriting");
    return;
  }
  if (file.print(message)){
    Serial.println("File written");
  }
  else{
    Serial.println("Write failed");
  }
  file.close();
  }
  /*
  void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}*/
void appendFile(fs::FS &fs, const char* path, const char* message){
  Serial.printf("Appending to file: %s\n",path);

  File file=fs.open(path,FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else{
    Serial.println("Appended failed");
    return;
  }
  file.close();
  

}

void setup()
{
    Serial.begin(115200);
    if (!SD.begin())
    {
        Serial.println("Card mount failed");
        return;
    }
    uint8_t cardType=SD.cardType();
    if (cardType==CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    createDir(SD,"/data");
    /*listDir(SD, "/", 0);
    createDir(SD, "/mydir");
    listDir(SD, "/", 0);
    removeDir(SD, "/mydir");
    listDir(SD, "/", 2);*/

    
    writeFile(SD, "/hello.txt", "Hello ");
    appendFile(SD, "/hello.txt", "World!\n");
    /*readFile(SD, "/hello.txt");
    deleteFile(SD, "/foo.txt");
    renameFile(SD, "/hello.txt", "/foo.txt");
    readFile(SD, "/foo.txt");
    testFileIO(SD, "/test.txt");*/
    
   
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
    
    
    acs712.calibrate();
    cal=acs712.calibrate();
}



void loop()
{
    /* if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }*/

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


  delay(300);
  
  
 // Serial.println(acs712.shape());
}


