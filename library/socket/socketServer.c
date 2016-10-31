/* A simple server in the internet domain using TCP.
myServer.c
D. Thiebaut
Adapted from http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
The port number used in 51717.
This code is compiled and run on the Raspberry as follows:

    g++ -o myServer myServer.c
    ./myServer

The server waits for a connection request from a client.
The server assumes the client will send positive integers, which it sends back multiplied by 2.
If the server receives -1 it closes the socket with the client.
If the server receives -2, it exits.
*/

#include "socketServer.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


static void error(char* msg) __attribute__ ((noreturn));

int sendData(int sockfd, char* szData, int nLength)
{
    int     n;

    if ((n = write(sockfd, szData, nLength)) < 0)
    {
        error(("ERROR writing to socket"));
    }
    return n;
}

int getData(int sockfd, char* szData, int nLength)
{
    int n;

    if ((n = read(sockfd, szData, nLength)) < 0)
    {
        error(("ERROR reading from socket"));
    }
    return n;
}

int socketServerOpen(int portno)
{
    int sockfd;
    struct sockaddr_in serv_addr;
    printf("using port #%d\n", portno);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error(("ERROR opening socket"));
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error(("ERROR on binding"));
    }

    listen(sockfd, 5);
    return sockfd;
}

int socketWaitForClient(int sockfd)
{
    int newsockfd;
    struct sockaddr_in cli_addr;
    int clilen;
    clilen = sizeof(cli_addr);
    printf("waiting for new client...\n");

    if ((newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, (socklen_t*) &clilen)) < 0)
    {
        error(("ERROR on accept"));
    }

    return newsockfd;
}

void socketServerClose(int fd)
{
    close(fd);
}

void error(char* msg)
{
    perror(msg);
    exit(1);
}


