#include "Queue.h"

Queue::Queue()
{
	array = new uint8_t[DEFAULT_QUEUE_SIZE];

	back = front = array; 

	count = 0;

}

Queue::Queue(int s)
{
	size = s;
	
	array = new uint8_t[size];

	back = front = array; 

	count = 0;

}

void Queue::add(uint8_t byte)
{

	if(!isFull())
	{	

    
		*front = byte;

//    Serial.print("In: ");
//    Serial.println((char)*front);

		int index = front - array;

//    Serial.print("index: ");
//    Serial.println(index); 

//    Serial.print("count: ");
//    Serial.println(count); 


		if(index >= size - 1)
		{
			front = array;
		}
		else
    {
			front++;
      
    }
		
		count ++;
	}

 
}

bool Queue::remove(uint8_t * outByte)
{

	if(!isEmpty())
	{
		*outByte = *back;

    
		int index = back - array;

		if(index >= size - 1)
		{
			back = array;
		}
		else
    {
//			Serial.println("back increase");
			back++;
    }
		count --;


		return true;

	}

	return false;

}

bool Queue::isFull()
{

  return back == front && count > 0;

}

bool Queue::isEmpty()
{
	return back == front && count == 0;
}

int Queue::getCount()
{

  return count;
  
}

