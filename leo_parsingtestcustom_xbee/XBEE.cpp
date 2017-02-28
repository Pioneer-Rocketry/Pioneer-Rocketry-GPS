
#include "XBEE.h"

#define WRITE_WAIT_MS 5

//XBEE::Platform boardType;

struct XBEE::ChipDef_t XBEE::chipDef;

XBEE::Platform boardType;

HardwareSerial* XBEE::cmdPort;

Queue * XBEE::input;

Queue * XBEE::output;

/**
 * serialInterface 1 indicates Serial. serialInterface 2 indicates Serial1....
 */
void XBEE::Initialize(long baud, int serialInterface)
{

	//Make these constants.
	if(baud > 115200)
		baud = 115200;
	else if(baud < 300)
		baud = 300;



  input = new Queue(MAX_BUFFER_SIZE);

  output = new Queue(MAX_BUFFER_SIZE);
  
  chipDef.boardType = Default;
  chipDef.numSerial = 1;

  #ifdef __AVR_ATtiny85__
  
  //This is Trinket, Gemma, Digispark
  chipDef.boardType = Digispark;
  chipDef.numSerial = 0;
  
  #elif _AVR_ATmega328_
  
  //This is an Uno.
  
  chipDef.boardType = Uno;
  chipDef.numSerial = 1;
  
  
  #elif __AVR_ATmega32U4__
  
  //This is a leonardo (micro)
  chipDef.boardType = Leonardo;
  chipDef.numSerial = 2;
  
  #elif __AVR_ATmega2560__
  
  //This is a MEGA
  chipDef.boardType = Mega;
  chipDef.numSerial = 5;
  
  #elif __SAM3X8E__
  
  //This is a DUE
  chipDef.boardType = Due;
  chipDef.numSerial = 5;
  
  #endif


  if(serialInterface > chipDef.numSerial)
  {
    serialInterface = chipDef.numSerial;
  }

  cmdPort = (HardwareSerial*)&Serial;

#ifdef __AVR_ATmega328__

#elif __AVR_ATmega32U4__

  if(serialInterface == 2)
    cmdPort = (HardwareSerial*)&Serial1;

#elif __AVR_ATmega2560__

  if(serialInterface == 2)
    cmdPort = (HardwareSerial*)&Serial1;
  else if(serialInterface == 3)
    cmdPort = (HardwareSerial*)&Serial2;
  else if(serialInterface == 4)
    cmdPort = (HardwareSerial*)&Serial3;
  else if(serialInterface == 5)
    cmdPort = (HardwareSerial*)&Serial4;
           
#elif __SAM3X8E__
    
  if(serialInterface == 2)
    cmdPort = (HardwareSerial*)&Serial1;
  else if(serialInterface == 3)
    cmdPort = (HardwareSerial*)&Serial2;
  else if(serialInterface == 4)
    cmdPort = (HardwareSerial*)&Serial3;
  //else if(serialInterface == 5)
  //  cmdPort = (HardwareSerial*)&Serial4;

#endif

  cmdPort ->  begin(baud);
}

void XBEE::Update()
{


  while(cmdPort->available() > 0)
  {
    uint8_t readByte = cmdPort->read();

    input->add(readByte);


  }

  uint8_t  writeByte;
  if(millis() - lastWrite >= WRITE_WAIT_MS)
  {	  
  	while(!output->isEmpty())
  	{
      output->remove(&writeByte);

      cmdPort->print((char) writeByte);
	  
    }
    lastWrite = millis();
  }
}

int XBEE::isAvailable()
{

  return input->getCount();
  
}

void XBEE::Write(char* buffer, int length)
{
  
  int count = 0;
  int arrayLength = strlen(buffer);
 
  while(count < length && count < arrayLength) 
  {
    output->add(buffer[count]);

    count++;
  
  }

}

int XBEE::Read(char* buffer, int length)
{
  int count = 0;
  for(; count < length && input->getCount() > 0; count++)
  {

    uint8_t * readByte;
    input->remove(readByte);
    cmdPort->write(*readByte);
    
    buffer[count] = *((char*)readByte);
    
  }

  return (count - 1);
  
}








