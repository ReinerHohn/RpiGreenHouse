#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcp3008.h"
#include "socketComm.h"
#include "pump.h"

/* this variable is our reference to the second thread */
pthread_t commandThread;
int commandSocketServerFd;
int commandSocketFd;

bool bCommandThread = true;
int  nCommand;
void *commandFunc(void *x_void_ptr)
{
    while(bCommandThread)
    {
        nCommand = getData( commandSocketFd );
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


    initMcp3008();
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
        adValue = getAdValue(0);
        fprintf(stderr, "AD-Wert Kanal %d ist %d \n", 0, adValue);
        sendData( connSock, adValue );
        sleep(1);
    }
    exitPump();
    exitMcp3008();

    socketServerClose(socketServerFd);

    if(pthread_join(commandThread, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;

    }
    return 0;
}
