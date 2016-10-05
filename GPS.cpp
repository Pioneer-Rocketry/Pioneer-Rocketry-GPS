#include "Arduino.h"
#include "HardwareSerial.h"
#include "GPS.h"

void GPS::Initialize()
{
	usingInterrupt = false;

	Serial.begin(115200);
	gps.begin(9600);
	mySerial.begin(9600);
	// uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	// uncomment this line to turn on only the "minimum recommended" data
	//GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	// For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
	// the parser doesn't care about other sentences at this time

	// Set the update rate
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	// For the parsing code to work nicely and have time to sort thru the data, and
	// print it out we don't suggest using anything higher than 1 Hz

	// Request updates on antenna status, comment out to keep quiet
	gps.sendCommand(PGCMD_ANTENNA);

	// the nice thing about this code is you can have a timer0 interrupt go off
	// every 1 millisecond, and read data from the GPS for you. that makes the
	// loop code a heck of a lot easier!

#ifdef __arm__
	usingInterrupt = false;  //NOTE - we don't want to use interrupts on the Due
#else
	useInterrupt(true);
#endif

	delay(1000);
	// Ask for firmware version
	mySerial.println(PMTK_Q_RELEASE);

#ifdef __AVR__
	// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
	SIGNAL(TIMER0_COMPA_vect) {
		char c = GPS.read();
		// if you want to debug, this is a good time to do it!
#ifdef UDR0
		if (GPSECHO)
			if (c) UDR0 = c;  
		// writing direct to UDR0 is much much faster than Serial.print 
		// but only one character can be written at a time. 
#endif
	}

	void useInterrupt(boolean v) {
		if (v) {
			// Timer0 is already used for millis() - we'll just interrupt somewhere
			// in the middle and call the "Compare A" function above
			OCR0A = 0xAF;
			TIMSK0 |= _BV(OCIE0A);
			usingInterrupt = true;
		} else {
			// do not call the interrupt function COMPA anymore
			TIMSK0 &= ~_BV(OCIE0A);
			usingInterrupt = false;
		}
	}
#endif //#ifdef__AVR__
}

void GPS::Update()
{
	timer = millis();
	// in case you are not using the interrupt above, you'll
	// need to 'hand query' the GPS, not suggested :(
	if (! usingInterrupt) {
		// read data from the GPS in the 'main loop'
		char c = gps.read();
		// if you want to debug, this is a good time to do it!
		if (GPSECHO)
			if (c) Serial.print(c);
	}

	// if a sentence is received, we can check the checksum, parse it...
	if (gps.newNMEAreceived()) {
		// a tricky thing here is if we print the NMEA sentence, or data
		// we end up not listening and catching other sentences! 
		// so be very wary if using OUTPUT_ALLDATA and trytng to print out data
		//Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

		if (!gps.parse(gps.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return;  // we can fail to parse a sentence in which case we should just wait for another
	}

	// if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	// approximately every 2 seconds or so, print out the current stats
	if (millis() - timer > 2000) { 
		timer = millis(); // reset the timer

		hour = gps.hour;
		minute = gps.minute;
		second = gps.seconds;
		millisecond = gps.milliseconds;
		day = gps.day;
		month = gps.month;
		year = gps.year;
		fix = (int)gps.fix;
		quality = (int)gps.fixquality;
		
		if (gps.fix) 
		{
			lat = gps.latitude;
			lon = gps.longitude;
			speed = gps.speed;
			angle = gps.angle;
			alt = gps.altitude;
			sat = gps.satellites;
		}
	}
}

void GPS::getTime(uint8_t& _day, uint8_t& _month, uint8_t& _year, uint8_t& _hour, uint8_t& _minute, uint8_t& _second, uint16_t& _millisecond)
{
	_day = day;
	_month = month;
	_year = year;
	_hour = hour;
	_minute = minute;
	_second = second;
	_millisecond = millisecond;
}

void GPS::getStats(int& _fix, int& _quality, float& _speed, float& _angle, float& _alt, uint8_t& _sat)
{
	_fix = fix;
	_quality = quality;
	_speed = speed;
	_angle = angle;
	_alt = alt;
	_sat = sat;
}

void GPS::GetCoordinates(float& latitude, float& longitude)
{
	latitude = lat;
	longitude = lon;
}