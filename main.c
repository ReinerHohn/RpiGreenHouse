#include <stdio.h>

#include "mcp3008.h"
#include "socketComm.h"

int main(int argc, char *argv[])
{
    int socketServerFd;
    int connSock;
    int adValue;

    socketServerFd = socketServerOpen(51718);
    initMcp3008();

    connSock = socketWaitForClient( socketServerFd);

    while(1)
    {
        adValue = getAdValue(0);
        fprintf(stderr, "AD-Wert Kanal %d ist %d \n", 0, adValue);
        sendData( connSock, adValue );
    }
    exitMcp3008();

    socketServerClose(socketServerFd);

    return 0;
}
