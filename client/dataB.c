#include "dataB.h"
#include "mysqlWrapper.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

static time_t t;

void dataBInitDb(MYSQL* connection)
{
    MYSQL_RES* result;
    result = sqlWrapQuery(connection, "CREATE DATABASE humidities;");
    result = sqlWrapQuery(connection, "USE humidities;");
    result = sqlWrapQuery(connection, "CREATE USER 'logger'@'localhost' IDENTIFIED BY 'password';");
    result = sqlWrapQuery(connection, "GRANT ALL PRIVILEGES ON humidities.* TO 'logger'@'localhost';");
    result = sqlWrapQuery(connection, "FLUSH PRIVILEGES;");
    result = sqlWrapQuery(connection, "CREATE TABLE humiditydata (dateandtime DATETIME, sensor1 VARCHAR(32), humidity1 DOUBLE, sensor2 VARCHAR(32), humidity2 DOUBLE, pump VARCHAR(32));");
}
MYSQL* dataBInit()
{
    int res;
    MYSQL* connection = sqlWrapOpen();
    res = sqlWrapConnect(connection, "localhost", "0", "root", "root", NULL);
    MYSQL_RES* result;
    result = sqlWrapQuery(connection, "USE humidities;");
    t = time(NULL);
    return connection;
}

void dataBExit(MYSQL* connection)
{
    sqlWrapClose(connection);
}

void dataBStoreHum(MYSQL* connection, int nSensor1, int dHumidity1, int nSensor2, int dHumidity2, int pump)
{
    char szSqlData[300];
    snprintf(szSqlData, sizeof(szSqlData), "INSERT INTO humiditydata SET dateandtime=NOW(), sensor1='%d', humidity1='%d', sensor2='%d', humidity2='%d', pump='%d';", nSensor1, dHumidity1, nSensor2, dHumidity2, pump);
    sqlWrapQuery(connection, szSqlData);
}

double dataAll(MYSQL* connection)
{
    MYSQL_RES* result;
    result = sqlWrapQuery(connection, "SELECT * FROM humiditydata;");
    sqlParse(connection, result);
}


long dataBgetMeanHum(MYSQL* connection, int nStart, int nCount)
{
    int i;
    MYSQL_RES* result;
    char szSqlQuery[300];
    snprintf(szSqlQuery, sizeof(szSqlQuery), "SELECT humidity1 FROM humiditydata  LIMIT %d,%d;", nStart, nCount);
    result = sqlWrapQuery(connection, szSqlQuery);
    long* l = (long*)malloc(nCount);
    long lmean = 0;
    i = 0;


    while ((l[i] = sqlWrapgetRow(connection, result)))
    {
        lmean += l[i];
        i++;
    }
    free(l);

    return lmean;
}

