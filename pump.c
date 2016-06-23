#include "gpio.h"
#include "pump.h"

#define PUMP_PIN 23

int initPump()
{
    if (-1 == GPIOExport(PUMP_PIN) )
	{
		return(1);
	}
    if (-1 == GPIODirection(PUMP_PIN, OUT) )
	{
		return(2);
	}
}


int exitPump()
{
    if (-1 == GPIOUnexport(PUMP_PIN) )
	{
		return(4);
	}
}
	
int pumpOn()
{
    if (-1 == GPIOWrite(PUMP_PIN, 1) )
	{
		return(3);
	}
}

int pumpOff()
{
    if (-1 == GPIOWrite(PUMP_PIN, 0) )
	{
		return(3);
	}
}
