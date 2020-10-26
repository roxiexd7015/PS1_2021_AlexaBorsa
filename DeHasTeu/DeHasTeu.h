#ifndef DeHasTeu
#define DeHasTeu

#include "Arduino.h"

class DHT
{
	int pin;
public:
	DHT(int _pin);
	void start();
	float Busu();
};

#endif // !DeHasTeu