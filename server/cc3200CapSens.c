#include "spiComm.h"

#include <stdio.h>
#include <time.h>

static int fd;

void initcc3200CapSens()
{
    fd = openSpi("/dev/spidev0.0", 100000, 0, 8, 0);
}

void exitcc3200CapSens()
{
    closeSpi(fd);
}

int getHumValue()
{
    uint8_t   auTxData[4] = {0x01, 0, 0, 0};
    uint8_t   auRxData[4] = {0, };
    int       nHumVal;

    nHumVal = 0;

    auTxData[0] = 1;
    auTxData[1] = 1;
    auTxData[2] = 1;
    auTxData[3] = 1;

    while(1)
    {
        transfer(fd, auTxData, ARRAY_SIZE(auTxData), auRxData, 100000, 0, 8);
        long ulFreq = (auRxData[0] << 24) | (auRxData[1] << 16) | (auRxData[2] << 8) | auRxData[3];

        if(ulFreq < 100000000)
        {
            fprintf(stderr, "Frequency : %d Hz Wassser \n\r", ulFreq);
        }
        else
        {
            fprintf(stderr, "Frequency : %d Hz Luft\n\r", ulFreq);
        }
        usleep(1000000);
    }
    transfer(fd, auTxData, ARRAY_SIZE(auTxData), auRxData, 100000, 0, 8);
    transfer(fd, auTxData, ARRAY_SIZE(auTxData), auRxData, 100000, 0, 8);
    return nHumVal;
}
