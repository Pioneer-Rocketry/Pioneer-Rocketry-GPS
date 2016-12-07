#ifndef _GPS_H
#define _GPS_H

#include "Arduino.h"
#include "C:\Program Files (x86)\Arduino\libraries\Adafruit_GPS\Adafruit_GPS.h"
//#include "HardwareSerial.h"
#include "SoftwareSerial.h"
#define mySerial Serial1
#define GPSECHO  true

/*typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;*/

class GPS
{
public:
	void Initialize();
	void Update();
	void GetTime(uint8_t& _day, uint8_t& _month, uint8_t& _year, uint8_t& _hour, uint8_t& _minute, uint8_t& _second, uint16_t& millisecond);
	void GetStats(int& _fix, int& _quality, float& _speed, float& _angle, float& _altitude, uint8_t& _sat);
	void GetCoordinates(float& latitude, float& longitude);
private:
	SoftwareSerial Serial1;
	boolean usingInterrupt;
	void useInterrupt(boolean);
	Adafruit_GPS gps /*(&mySerial)*/;
	float lat;
	float lon;
	uint32_t timer;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint16_t millisecond;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	int fix;
	int quality;
	float speed;
	float angle;
	float alt;
	uint8_t sat;
};

#endif
