#include "spiComm.h"

static int fd;

int calculateAdValue(char* pszRxData, int a2dChannel);

void initMcp3008()
{
	fd = openSpi("/dev/spidev0.0", 500000, 0, 8, 0);
}

void exitMcp3008()
{
	closeSpi(fd);
}

int getAdValue(int a2dChannel)
{
    unsigned char   auTxData[3];
    uint8_t         auRxData[ARRAY_SIZE(auTxData)] = {0, };
    int             a2dVal;

    a2dVal = 0;

    auTxData[0] = 1;  //  first byte transmitted -> start bit
    auTxData[1] = 0b10000000 |( ((a2dChannel & 7) << 4)); // second byte transmitted -> (SGL/DIF = 1, D2=D1=D0=0)
    auTxData[2] = 0; // third byte transmitted....don't care

    transfer(fd, auTxData, ARRAY_SIZE(auTxData), auRxData, 500000, 0, 8);

    a2dVal = calculateAdValue(auRxData, a2dChannel);

    return a2dVal;
}

int calculateAdValue(char* pszRxData, int a2dChannel)
{
    int a2dVal = 0;

    a2dVal = 0;
    a2dVal = (pszRxData[1]<< 8) & 0b1100000000; //merge data[1] & data[2] to get result
    a2dVal |=  (pszRxData[2] & 0xff);

    return a2dVal;
}
