#ifndef DATAB_H
#define DATAB_H

#include "mysql.h"

void dataBInitDb(MYSQL* connection);
MYSQL* dataBInit();
void dataBExit(MYSQL* connection);
void dataBStoreHum(MYSQL* connection, int nSensor1, double dHumidity1, int nSensor2, double dHumidity2, int pump);
double dataAll(MYSQL* connection);
long dataBgetMeanHum(MYSQL* connection, int nStart, int nCount);

#endif //DATAB_H
