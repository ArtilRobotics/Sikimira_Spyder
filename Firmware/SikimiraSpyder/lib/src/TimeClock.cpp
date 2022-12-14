#include <Arduino.h>
#include <TimeClock.h>
#include <WiFi.h>
#include "time.h"
#include <PermanentValues.h>
#include <ESP32Time.h>

ESP32Time rtc;

const char* ssid="CasaPP";
const char* password="Casa151098#";
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec=-5*3600;
const int daylightOffset_sec=0;

PermanentValues SetValues;

void TimeClock::init()
{
  initTime();
  printLocalTime();
}

void TimeClock::setTimezone(String timezone)
{
  // Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ", timezone.c_str(), 1); //  Now adjust the COT5.  Clock settings are adjusted to show the new local time
  tzset();
}

void TimeClock::initTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo); 
  }
}

void TimeClock::printLocalTime()
{
  struct tm timeinfo = rtc.getTimeStruct();
  Serial.println(&timeinfo, "%H:%M:%S");
}

boolean array_cmp(int *a, int *b, int len_a, int len_b)
{
  int n;

  // if their lengths are different, return false
  if (len_a != len_b)
    return false;

  // test each element to be the same. if not, return false
  for (n = 0; n < len_a; n++)
    if (a[n] != b[n])
      return false;

  // ok, if we have not returned yet, they are equal :)
  return true;
}

// bool TimeClock::Compare_Time(){
//   struct tm timeinfo;
//   bool Status=false;
//   if(!getLocalTime(&timeinfo)){}

//    datoCC[0] = timeinfo.tm_hour;
//    datoCC[1] = timeinfo.tm_min;

//    if(datoCC[0]>datoC[0] || datoCC[0]==datoC[0] && datoCC[1]>=datoC[1]){
//       if(datoCC[0]>datoC[0]){
//         Serial.print(datoCC[0]);
//         Serial.print(" > ");
//         Serial.println(datoC[0]);
//       }

//       if(datoCC[0]==datoC[0] && datoCC[1]>=datoC[1]){
//         Serial.print(datoCC[0]);
//         Serial.print(" == ");
//         Serial.print(datoC[0]);
//         Serial.print(" && ");
//         Serial.print(datoCC[1]);
//         Serial.print(" >= ");
//         Serial.println(datoC[1]);
//       }

//     Status = true;
//    }

//    //Serial.print(datoCC[0]);
//    //Serial.print(":");
//    //Serial.println(datoCC[1]);
//    //Serial.println(Status);

//   return Status;
// }

bool TimeClock::Compare_Time()
{
  
  bool Status = false;

  struct tm timeinfo = rtc.getTimeStruct();

  datoCC[0] = timeinfo.tm_hour;
  datoCC[1] = timeinfo.tm_min;

  if (datoCC[0] > datoTON[0] && datoCC[0] < datoTOFF[0])
    Status = true;

  if (datoCC[0] >= datoTON[0] && datoCC[0] <= datoTOFF[0])
  {
    if (datoCC[0] == datoTON[0] && datoCC[0] == datoTOFF[0])
    {
      if (datoCC[1] >= datoTON[1] && datoCC[1] < datoTOFF[1])
      {
        Status = true;
      }
    }
    if (datoCC[0] > datoTON[0] && datoCC[0] == datoTOFF[0])
    {
      if (datoCC[1] < datoTOFF[1])
      {
        Status = true;
      }
    }
    if (datoCC[0] == datoTON[0] && datoCC[0] < datoTOFF[0])
    {
      if (datoCC[1] >= datoTON[1])
      {
        Status = true;
      }
    }
  }

  if (datoCC[0] == 0 && datoCC[1]==0 ){ESP.restart();}
  

  return Status;
  // Serial.print(datoCC[0]);
  // Serial.print(":");
  // Serial.println(datoCC[1]);
  // Serial.println(Status);
}

void TimeClock::setTimeON(int datoC[2])
{

  datoTON[0] = datoC[0];
  datoTON[1] = datoC[1];

  Serial.print("Set ON time to: ");
  Serial.print(datoTON[0]);
  Serial.print(":");
  Serial.println(datoTON[1]);

  SetValues.WR_time(true, char(datoTON[0]), char(datoTON[1]), true);
}

void TimeClock::setTimeOFF(int datoC[2])
{

  datoTOFF[0] = datoC[0];
  datoTOFF[1] = datoC[1];

  Serial.print("Set OFF time to: ");
  Serial.print(datoTOFF[0]);
  Serial.print(":");
  Serial.println(datoTOFF[1]);

  SetValues.WR_time(false, char(datoTOFF[0]), char(datoTOFF[1]), true);
}

int TimeClock::TodayDay()
{

  struct tm timeinfo = rtc.getTimeStruct();

  int Day = timeinfo.tm_wday;
  return Day;
}

