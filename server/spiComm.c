/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "spiComm.h"

static void pabort(const char *s)
{
	perror(s);
	abort();
}


int transfer(int fd, const char* pszTxData, uint8_t u8TxLength, char* pszRxData, uint32_t u32Speed, uint16_t u16Delay, uint8_t u8Bits)
{
    int ret;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)pszTxData,
                .rx_buf = (unsigned long)pszRxData,
                .len = u8TxLength,
                .delay_usecs = u16Delay,
                .speed_hz = u32Speed,
                .bits_per_word = u8Bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        pabort("can't send spi message");
    }
}

int openSpi(const char *szDevice, uint32_t u32Speed, uint16_t u16Delay, uint8_t u8Bits, uint8_t u8Mode)
{
    int fd;
    int ret;

    fd = open(szDevice, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    /*
     * spi mode
     */
    ret = ioctl(fd, SPI_IOC_WR_MODE, &u8Mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &u8Mode);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &u8Bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &u8Bits);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &u32Speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &u32Speed);
    if (ret == -1)
        pabort("can't get max speed hz");

    printf("spi mode: %d\n", u8Mode);
    printf("bits per word: %d\n", u8Bits);
    printf("max speed: %d Hz (%d KHz)\n", u32Speed, u32Speed/1000);

    return fd;
}

void closeSpi(int fd)
{
     close(fd);
}


static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word \n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[], char* szDevice, uint32_t* pu32Speed, uint16_t* pu16Delay, uint8_t* pu8Bits, uint8_t* pu8Mode)
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:lHOLC3NR", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
            szDevice = optarg;
			break;
		case 's':
            *pu32Speed = atoi(optarg);
			break;
		case 'd':
            *pu16Delay = atoi(optarg);
			break;
		case 'b':
            *pu8Bits = atoi(optarg);
			break;
		case 'l':
            *pu8Mode |= SPI_LOOP;
			break;
		case 'H':
            *pu8Mode |= SPI_CPHA;
			break;
		case 'O':
            *pu8Mode |= SPI_CPOL;
			break;
		case 'L':
            *pu8Mode |= SPI_LSB_FIRST;
			break;
		case 'C':
            *pu8Mode |= SPI_CS_HIGH;
			break;
		case '3':
            *pu8Mode |= SPI_3WIRE;
			break;
		case 'N':
            *pu8Mode |= SPI_NO_CS;
			break;
		case 'R':
            *pu8Mode |= SPI_READY;
			break;
		default:
			print_usage(argv[0]);
			break;
		}
	}
}

