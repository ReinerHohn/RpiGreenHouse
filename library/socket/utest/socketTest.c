#include "socketServer.h"
#include "socketClient.h"

#include "pthread.h"
#include <stdio.h>

int socketServerFd;
int socketServer;

#define  recLength 10
#define sendLength recLength

static int nPort = 20005;
static char recBuf[recLength];
char sendBuf[sendLength];

void* fserverThread(void* arg)
{
    while(1)
    {
        socketServer = socketServerWaitForClient(socketServerFd);
        socketServerGetData(socketServer, recBuf, recLength);
    }
}

void sockTransferTest(void)
{
    struct hostent* phostent;
    int socketClientFd;

    pthread_t tserverThread;

    sprintf(sendBuf, "Hallo Wel");
    sprintf(recBuf, "Reiner Ho");

    socketServerFd = socketServerOpen(nPort);
    phostent = socketClientOpen(&socketClientFd, "localhost");

    if (pthread_create(&tserverThread, NULL, fserverThread, NULL))
    {
        perror("Error creating thread\n");
        return;
    }

    socketClientConnect(socketClientFd, phostent, nPort);
    socketClientSendData(socketClientFd, sendBuf, sendLength );

    fprintf(stderr, "sendBuf: %s", sendBuf);
    fprintf(stderr, "recBuf: %s", recBuf);

    socketClientClose(socketClientFd);
    socketServerClose(socketServerFd);
}
