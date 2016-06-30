#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "socketClient.h"
#include "fifo.h"

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

void * calcRecThread(void* arg)
{
    static int val;


    while( bDataThread) //!CON_BUF_EMPTY(Fifo) )
    {
        if( pthread_mutex_trylock(&count_mutex) == 0 )
        {
            val = var; //CON_BUF_RD(Fifo);
            /* only enter if new value was set */
            if(var > 600)
            {
                pthread_cond_signal(&count_threshold_cv);
                var = 0;
                sendData(sockfdCommand, pumpOn); //
            }
            else if(var > 0 && var < 600)
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

    struct hostent* phostent;
    Fifo = &TFifo;
    CON_BUF_RESET(Fifo);
    n = 0;

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


