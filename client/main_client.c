#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "socketClient.h"
#include "fifo.h"
#include "dataB.h"

char arecData[1024];
char asendData[1024];

static bool bDataThread;
static int sockfdData;

static CON_Fifo_t* Fifo;
static CON_Fifo_t TFifo;

static int g_var;

static pthread_mutex_t count_mutex;
static pthread_cond_t count_threshold_cv;

static int pumpOn = 0;
static int pumpOff = 1;

static int nGrenze = 550;
static MYSQL* connection;

int sockfdCommand;

static int g_naData[32];
int i;

int nRecLength = 9;
int nSendLength = 2;

void* dataRecThread(void* arg)
{
    char pTemp[3];
    while (bDataThread)
    {
        recData(sockfdData, arecData, nRecLength);
       // pTemp = (char*)malloc(3);
        pthread_mutex_lock(&count_mutex);
        memcpy(pTemp, arecData, 3);
        g_naData[0] = strtoul(pTemp, NULL, 10);
        memcpy(pTemp, arecData + 4, 3);
        g_naData[1] = strtoul(pTemp, NULL, 10);

        char* pch;
       // pch = strtok(pTemp, ";");

        //while (pch != NULL)
        {
          //  g_naData[i] = strtoul(pTemp, NULL, 10);
           // fprintf(stderr, "d\n", g_naData[i]);
          //  pch = strtok(NULL, ";");

            //i++;
        }
        i = 0;
        fprintf(stderr, "Feuchte1  %d, Feuchte 2 %d \n", g_naData[0], g_naData[1]);
        pthread_mutex_unlock(&count_mutex);

    }
}



void* calcRecThread(void* arg)
{
    while (bDataThread) //!CON_BUF_EMPTY(Fifo) )
    {
        if (pthread_mutex_trylock(&count_mutex) == 0)
        {
            dataBStoreHum(connection, 0, g_naData[0], 1, g_naData[1], pumpOn);
            /* only enter if new value was set */
            if (g_naData[0] > nGrenze)
            {
                pthread_cond_signal(&count_threshold_cv);

                // Wenn sicherheitssensor im Wasser
                if (g_naData[1] < 500)
                {
                    sprintf(asendData, "%d", pumpOn);
                    sendData(sockfdCommand, asendData, nSendLength); //
                }
            }
            else if (g_naData[0] > 0 && g_naData[0] < nGrenze)
            {

                sprintf(asendData, "%d", pumpOff);
                sendData(sockfdCommand, asendData, nSendLength);
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

    connection = dataBInit();
    //dataBInitDb(connection);
    //dataBgetMeanHum(connection, 500, 1);
    phostent = socketClientOpen(&sockfdCommand, "192.168.178.79");
    socketClientConnect(sockfdCommand, phostent, 51719);
    phostent = socketClientOpen(&sockfdData, "192.168.178.79");
    socketClientConnect(sockfdData, phostent, 51718);
    bDataThread = true;

    if (pthread_create(&dataThread, NULL, dataRecThread, NULL))
    {
        perror("Error creating thread\n");
        return 1;
    }

    if (pthread_create(&dataThread, NULL, calcRecThread, NULL))
    {
        perror("Error creating thread\n");
        return 1;
    }

    while (1)
    {
        pthread_mutex_lock(&count_mutex);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
        pthread_mutex_unlock(&count_mutex);
    }

    socketClientClose(sockfdData);
    socketClientClose(sockfdCommand);
    return 0;
}

