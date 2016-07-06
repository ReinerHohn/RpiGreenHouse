#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "socketClient.h"
#include "fifo.h"
#include "mysqlWrapper.h"

static int data;
static bool bDataThread;
static int sockfdData;

static CON_Fifo_t* Fifo;
static CON_Fifo_t TFifo;

static int var;

static pthread_mutex_t count_mutex;
static pthread_cond_t count_threshold_cv;

static int pumpOn = 0;
static int pumpOff = 1;

int sockfdCommand;

void *dataRecThread(void* arg)
{
    while(bDataThread)
    {
        data = getData(sockfdData);

        pthread_mutex_lock(&count_mutex);
        //if( !CON_BUF_FULL(Fifo) )
        {
            var = data;
        //    CON_BUF_WR(Fifo, data);
        }
        pthread_mutex_unlock(&count_mutex);

        fprintf(stderr, "Feuchte  %d\n", data);
    }
}

static int nGrenze = 400;

void * calcRecThread(void* arg)
{
    static int val;


    while( bDataThread) //!CON_BUF_EMPTY(Fifo) )
    {
        if( pthread_mutex_trylock(&count_mutex) == 0 )
        {
            val = var; //CON_BUF_RD(Fifo);
            /* only enter if new value was set */
            if(var > nGrenze)
            {
                pthread_cond_signal(&count_threshold_cv);
                var = 0;
                sendData(sockfdCommand, pumpOn); //
            }
            else if(var > 0 && var < nGrenze)
            {
                var = 0;
                sendData(sockfdCommand, pumpOff); //
            }
            pthread_mutex_unlock(&count_mutex);
         }
    }
}

int main(int argc, char* argv[])
{
    pthread_t dataThread;
    pthread_t calcThread;

    int n;
    int res;

    struct hostent* phostent;
    Fifo = &TFifo;
    CON_BUF_RESET(Fifo);
    n = 0;

    MYSQL* connection = sqlWrapOpen();
    res = sqlWrapConnect(connection, "localhost", "0", "root", "root", NULL);

    MYSQL_RES* result;
    result = sqlWrapQuery(connection, "CREATE DATABASE temperatures;");
    result = sqlWrapQuery(connection, "USE temperatures;");
    result = sqlWrapQuery(connection, "CREATE USER 'logger'@'localhost' IDENTIFIED BY 'password';");
    result = sqlWrapQuery(connection, "GRANT ALL PRIVILEGES ON temperatures.* TO 'logger'@'localhost';");
    result = sqlWrapQuery(connection, "FLUSH PRIVILEGES;");
    result = sqlWrapQuery(connection, "USE temperatures;");

    result = sqlWrapQuery(connection, "INSERT INTO temperaturedata SET dateandtime='1234', sensor='1', humidity='450';");


    result = sqlWrapQuery(connection, "CREATE TABLE temperaturedata (dateandtime DATETIME, sensor VARCHAR(32), humidity DOUBLE);");

    result = sqlWrapQuery(connection, "SELECT * FROM temperaturedata;");


    sqlParse(connection, result);
    sqlWrapClose(connection);


    phostent = socketClientOpen(&sockfdCommand, "192.168.178.79");
    socketClientConnect(sockfdCommand, phostent, 51719);

    phostent = socketClientOpen(&sockfdData, "192.168.178.79");
    socketClientConnect(sockfdData, phostent, 51718);

    bDataThread = true;
    if(pthread_create(&dataThread, NULL, dataRecThread, NULL) )
    {
        perror("Error creating thread\n");
        return 1;
    }
    if(pthread_create(&dataThread, NULL, calcRecThread, NULL) )
    {
        perror("Error creating thread\n");
        return 1;
    }

    while(1)
    {
        pthread_mutex_lock(&count_mutex);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);

        pthread_mutex_unlock(&count_mutex);
    }
    socketClientClose(sockfdData);
    socketClientClose(sockfdCommand);
    return 0;
}

