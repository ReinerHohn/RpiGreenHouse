#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include <stdlib.h>
#include <unistd.h>

#include "cc3200CapSens.h"
#include "socketComm.h"
#include "pump.h"

/* this variable is our reference to the second thread */
pthread_t commandThread;
int commandSocketServerFd;
int commandSocketFd;

#define  recLength 2
#define sendLength 9

static char recBuf[recLength];
char sendBuf[sendLength];

bool bCommandThread = true;
int  nCommand;

void *commandFunc(void *x_void_ptr)
{
    while(bCommandThread)
    {
        getData( commandSocketFd, recBuf, recLength - 1);
        nCommand = strtoul(recBuf, NULL, 10);
        switch(nCommand)
        {
            case 0:
                fprintf(stderr, "Pump on \n");
                pumpOn();

            break;

            case 1:
                fprintf(stderr, "Pump off \n");
                pumpOff();
            break;

            default:
                fprintf(stderr, "Nix kam an \n");
            break;
        }
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    int socketServerFd;
    int connSock;
    int adValue;



    initcc3200CapSens();
    getHumValue();



    initPump();

    socketServerFd = socketServerOpen(51718);
    commandSocketServerFd = socketServerOpen(51719);

    connSock = socketWaitForClient(socketServerFd);
    commandSocketFd= socketWaitForClient(commandSocketServerFd);

    bCommandThread = true;
    if(pthread_create(&commandThread, NULL, commandFunc, NULL) )
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    while(1)
    {
        //snprintf(sendBuf, sizeof(sendBuf), "%d;%d;",getAdValue(0), getAdValue(1) );
        //fprintf(stderr, "AD-Wert Kanal 0 ist %d, Kanal 1 ist %d \n", getAdValue(0), getAdValue(1));
        sendData( connSock, sendBuf, sendLength );
        sleep(1);
    }
    exitPump();

    socketServerClose(socketServerFd);

    if(pthread_join(commandThread, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
    return 0;
}
