#include "Arduino.h"
#include "Queue.h"

#define MAX_BUFFER_SIZE 128
 
class XBEE
{
  
public:

/*This platform stuff should be implmeneted somewhere else.*/
  enum Platform {Uno, Leonardo, Mega, Due, Digispark, Default};

  struct ChipDef_t {
    Platform boardType;
    int numSerial;
  };

	/**
	* Sets up the XBEE interface. Taking into account board type.
	*/
	void Initialize(long baud, int serialInterface);

	/**
	* Updates and reads from the xbee to the internal buffer.
	*/
	void Update();

	/**
	* Return the number of bytes available for reading.
	*/
	int isAvailable();

	/**
	* Writes to the XBEE with the specified buffer
	*/
	void Write(char* buffer, int length);

	/**
	* Reads from the XBEE to the buffer for the specified size.
	* Returns number of lines read.
	*/
	int Read(char* buffer, int length);

	/**
	* Reads from the XBEE to the buffer until a new line character
	* Returns number of characters read.
	*/
	int ReadLine(char& buffer); 

  /**
   * Calls the underlying Serial.Print function
   */
   long print(char * message){return cmdPort->print(message);}

   long print(int message, int format){return cmdPort->print(message, format);}

   long print(float message, int format){return cmdPort->print(message, format);}

	//int ReadCommand(String command, String& action)

	Platform getBoardType();

private:

  char * line;
  int lineLength;
  long lastWrite = 0;

  //Buffers used to read from and write to the XBEE.
  char * outBuffer;
  char * inBuffer;

  static Platform boardType;

  static struct ChipDef_t chipDef;

  static HardwareSerial* cmdPort;

  static Queue * input;

  static Queue * output;


};
