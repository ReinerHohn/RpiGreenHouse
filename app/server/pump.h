#ifndef PUMP_H
#define PUMP_H

/**************************************************************************//**
 * @file   pump.h
 *
 * @brief  Gpio-Funktionen zum Ansteuern der Pumpen
 * 
 * @author Michael Dick	
 * @date   9.10.2016
 *****************************************************************************/

int initPump();
int exitPump();
	
int pumpOn();
int pumpOff();

#endif //PUMP_H
