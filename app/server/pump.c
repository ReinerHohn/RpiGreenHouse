/**************************************************************************//**
 * @file   pump.c
 *
 * @brief  Gpio-Funktionen zum Ansteuern der Pumpen
 * 
 * @author Michael Dick	
 * @date   9.10.2016
 *****************************************************************************/ 

#include "gpio.h"
#include "pump.h"

#define PUMP_PIN 23

/**************************************************************************//**
 * @brief  Setzt die Pins für die Pumpe
 * @return Non-Zero if error   
 *****************************************************************************/ 
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
    return 0;
}


int exitPump()
{
    if (-1 == GPIOUnexport(PUMP_PIN) )
	{
		return(4);
	}
    return 0;
}
	
int pumpOn()
{
    if (-1 == GPIOWrite(PUMP_PIN, 1) )
	{
		return(3);
	}
    return 0;
}

int pumpOff()
{
    if (-1 == GPIOWrite(PUMP_PIN, 0) )
	{
		return(3);
	}
    return 0;
}
