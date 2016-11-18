#ifndef SOCKET_COMM
#define SOCKET_COMM

int socketServerOpen(int portno);
int socketServerWaitForClient( int sockfd);
void socketServerClose(int fd);

int socketServerSendData(int sockfd, char* szData, int nLength);
int socketServerGetData(int sockfd, char* szData, int nLength);

#endif //SOCKET_COMM
