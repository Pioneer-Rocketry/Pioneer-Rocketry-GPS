/*
 Name:		GPS2.ino
 Created:	12/6/2016 6:07:57 PM
 Author:	Adam Nielsen
*/

#include "Arduino.h"
#include "Adafruit_GPS.h"
#include "SoftwareSerial.h"
#include <SPI.h>
#include <SD.h>
#include "GPS.h"


GPS * pr_GPS; 
// the setup function runs once when you press reset or power the board
void setup() {
  pr_GPS = new GPS();
  pr_GPS->Initialize();
}

float lat, lon;

uint8_t day, month, year, hour, minute, second;
uint16_t millisecond;

bool gps_fix;

int gps_quality;
float gps_speed, gps_angle, gps_altitude;

uint8_t gps_sat;



// the loop function runs over and over again until power down or reset
void loop() {


  pr_GPS->GetCoordinates(lat, lon);
  
  Serial.print("Location - ");
  Serial.print("Lat: ");
  Serial.print(lat);
  Serial.print(" Long: ");
  Serial.println(lon);

  pr_GPS->GetTime(day, month, year, hour, minute, second, millisecond);



  Serial.print("Date - ");
  Serial.print(day);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(year);
  
  Serial.print(" ");
  
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(";");
  Serial.print(second);
  Serial.print(".");
  Serial.println(millisecond);

  uint8_t gps_sat;

  pr_GPS->GetStats(gps_fix, gps_quality, gps_speed, gps_angle, gps_altitude, gps_sat);
  
  Serial.print("Fix: ");
  Serial.println(gps_fix);
  
  Serial.print("Quality: ");
  Serial.println(gps_quality);
  
  Serial.print("Speed: ");
  Serial.println(gps_speed);
  
  Serial.print("Angle: ");
  Serial.println(gps_angle);
  
  Serial.print("Altitude: ");
  Serial.println(gps_altitude);
  Serial.println();
  Serial.println();

  pr_GPS->Update();

  
}
