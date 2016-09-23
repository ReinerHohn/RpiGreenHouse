#ifndef SOCKET_COMM
#define SOCKET_COMM

int socketServerOpen(int portno);
int socketWaitForClient( int sockfd);
void socketServerClose(int fd);

int sendData(int sockfd, char* szData, int nLength);
int getData(int sockfd, char* szData, int nLength);

#endif //SOCKET_COMM
