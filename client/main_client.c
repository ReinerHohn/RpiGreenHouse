#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "socketClient.h"

static int data;
static bool bDataThread;
static int sockfdData;

void *dataThreadFunc(void* arg)
{
    while(bDataThread)
    {
        data = getData(sockfdData);
        fprintf(stderr, "Feuchte  %d\n", data);
    }
}

int main(int argc, char* argv[])
{
    pthread_t dataThread;

    int sockfdCommand;

    int n;

    struct hostent* phostent;

    phostent = socketClientOpen(&sockfdCommand, "192.168.178.61");
    socketClientConnect(sockfdCommand, phostent, 51719);

    phostent = socketClientOpen(&sockfdData, "192.168.178.61");
    socketClientConnect(sockfdData, phostent, 51718);

    bDataThread = true;
    if(pthread_create(&dataThread, NULL, dataThreadFunc, NULL) )
    {
        perror("Error creating thread\n");
        return 1;
    }

    for (n = 0; 1; n++)
    {
        sendData(sockfdCommand, n % 2);
        sleep(3);
    }

    socketClientClose(sockfdData);
    socketClientClose(sockfdCommand);
    return 0;
}


