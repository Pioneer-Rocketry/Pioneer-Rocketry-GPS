#ifndef _GPS_H
#define _GPS_H

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include "Adafruit_GPS.h"

const int chipSelect = 10;

class GPS{
  
public:
        GPS();
	void Initialize();
	void Update();
	void GetTime(uint8_t& day, uint8_t& month, uint8_t& year, uint8_t& hour, uint8_t& minute, uint8_t& second, uint16_t& millisecond);
	void GetStats(bool& fix, int& quality, float& speed, float& angle, float& altitude, uint8_t& sat);
	void GetCoordinates(float& lat, float& lon);
        

private:

	const int chipSelect = 10;
	// If using software serial, keep these lines enabled
	// (you can change the pin numbers to match your wiring):
	Adafruit_GPS * adafruit_gps;
	// If using hardware serial, comment
	// out the above two lines and enable these two lines instead:
	//Adafruit_GPS GPS(&Serial1);
	//HardwareSerial mySerial = Serial1;
	uint32_t timer;
	// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
	// Set to 'true' if you want to debug and listen to the raw GPS sentences
	#define GPSECHO  false
        SoftwareSerial * mySerial;//(8,7);
        File myFile;

};

#endif
