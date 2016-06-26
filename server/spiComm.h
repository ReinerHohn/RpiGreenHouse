#ifndef SPI_COMM
#define SPI_COMM

#include <stdint.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

int openSpi(const char *szDevice, uint32_t u32Speed, uint16_t u16Delay, uint8_t u8Bits, uint8_t u8Mode);
int transfer(int fd, const char* pszTxData, uint8_t u8TxLength, char* pszRxData, uint32_t u32Speed, uint16_t u16Delay, uint8_t u8Bits);
void closeSpi(int fd);

#endif //SPI_COMM
