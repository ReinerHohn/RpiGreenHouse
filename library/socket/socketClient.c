#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

void error(char* msg);

struct hostent* socketClientOpen(int* sockfd, char *szServerIp)
{
    struct hostent *server;

    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error(("ERROR opening socket"));
    }

    if ((server = gethostbyname(szServerIp)) == NULL)
    {
        error(("ERROR, no such host\n"));
    }
    return server;
}

void socketClientConnect(int sockfd, struct hostent* server, int portno)
{
    struct sockaddr_in serv_addr;

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error(("ERROR connecting"));
    }
}

int sendData(int sockfd, char *buffer, int nLength)
{
    int n;

    if ((n = write(sockfd, buffer, strlen(buffer))) < 0)
    {
        error(("ERROR writing to socket"));
    }

    return n;
}

int recData(int sockfd, char* buffer, int nLength)
{
    int n;

    if ((n = read(sockfd, buffer, nLength)) < 0)
    {
        error(("ERROR reading from socket"));
    }

    return n;
}

void socketClientClose(int sockfd)
{
    close(sockfd);
}

void error(char* msg)
{
    perror(msg);
    exit(0);
}
