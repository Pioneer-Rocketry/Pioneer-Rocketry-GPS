#include "Arduino.h"

#define DEFAULT_QUEUE_SIZE 60

class Queue
{

public:

	Queue();

	Queue(int size);

	void add(uint8_t byte);

	bool remove(uint8_t * outByte);

	bool isFull();

	bool isEmpty();

  int getCount();

private:

	uint8_t * array;

	uint8_t * front;

	uint8_t * back;

	int size;

	int count;


};
