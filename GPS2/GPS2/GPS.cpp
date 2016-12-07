#include "GPS.h"


GPS::GPS()
{
  
  mySerial = new SoftwareSerial(8,7);
  adafruit_gps = new Adafruit_GPS(mySerial);
  
}

void GPS::Initialize()
{
        //mySerial = new SoftwareSerial(8,7);
        
        //adafruit_gps = Adafruit_GPS(*mySerial);
  	/*
	while (!Serial) //SD
	{
	//;  wait for serial port to connect. Needed for native USB port only
	}
	*/

	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);
	if (!SD.begin(10)) 
	{
		Serial.println("initialization failed!");
		return;
	}
	Serial.println("initialization done.");

	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	myFile = SD.open("data.txt", FILE_WRITE);                                    //WHERE YOU CHANGE THE NAME OF THE FILE
	myFile.close();
	// if the file opened okay, write to it:

	if (myFile) 
	{
		Serial.print("Writing to data.txt...");
		myFile.print("NEW DATA SERIES");
		//close the file:
		myFile.close();
	}
	// Needs to be able to save i to memory then retrieve it when turned off then updated

	myFile = SD.open("data.txt", FILE_WRITE);
	myFile.println("-----------NEW DATASET-----------");
	//close the file:
	myFile.close();
	Serial.println("done.");



	//END SD


	// connect at 115200 so we can read the gps fast enough and echo without dropping chars
	// also spit it out
	Serial.begin(115200);
	delay(5000);
	Serial.println("Adafruit gps enabled");

	// 9600 NMEA is the default baud rate for Adafruit MTK gps's- some use 4800
	adafruit_gps->begin(9600);

	// uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
	adafruit_gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	// uncomment this line to turn on only the "minimum recommended" data
	//adafruit_gps->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	// For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
	// the parser doesn't care about other sentences at this time

	// Set the update rate
	adafruit_gps->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	// For the parsing code to work nicely and have time to sort thru the data, and
	// print it out we don't suggest using anything higher than 1 Hz

	// Request updates on antenna status, comment out to keep quiet
	adafruit_gps->sendCommand(PGCMD_ANTENNA);

	delay(1000);
	// Ask for firmware version
	mySerial->println(PMTK_Q_RELEASE);

	timer = millis();
}

void GPS::Update()
{
	char c = adafruit_gps->read();
	// if you want to debug, this is a good time to do it!
	if ((c) && (GPSECHO))
		Serial.write(c); 

	// if a sentence is received, we can check the checksum, parse it...
	if (adafruit_gps->newNMEAreceived()) {
		// a tricky thing here is if we print the NMEA sentence, or data
		// we end up not listening and catching other sentences! 
		// so be very wary if using OUTPUT_ALLDATA and trytng to print out data
		//Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

		if (!adafruit_gps->parse(adafruit_gps->lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return;  // we can fail to parse a sentence in which case we should just wait for another
	}

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds or so, print out the current stats
	if (millis() - timer > 2000) { 
		timer = millis(); // reset the timer
	}
}

void GPS::GetTime(uint8_t& day, uint8_t& month, uint8_t& year, uint8_t& hour, uint8_t& minute, uint8_t& second, uint16_t& millisecond)
{
	day = adafruit_gps->day;
	month = adafruit_gps->month;
	year = adafruit_gps->year;
	hour = adafruit_gps->hour;
	minute = adafruit_gps->minute;
	second = adafruit_gps->seconds;
	millisecond = adafruit_gps->milliseconds;
}

void GPS::GetStats(bool& fix, int& quality, float& speed, float& angle, float& altitude, uint8_t& sat)
{
	quality = adafruit_gps->fixquality;
	if(adafruit_gps->fix)
	{
		speed = adafruit_gps->speed;
		angle = adafruit_gps->angle;
		altitude = adafruit_gps->altitude;
	}

	else
	{
		speed = -1;
		angle = -1;
		altitude = -1;
	}
}

void GPS::GetCoordinates(float& lat, float& lon)
{
	if(adafruit_gps->fix)
	{
		lat = adafruit_gps->latitude;
		lon = adafruit_gps->longitude;
	}
	else
	{
		lat = -1;
		lon = -1;
	}
}
