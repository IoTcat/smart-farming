#ifndef GY30_H_
#define GY30_H_
#include <Wire.h>
#include <Arduino.h>

class GY30
{
	#define ADDR 0b0100011	
	
	private:
		
	public:
		int getLight();
};

#endif

