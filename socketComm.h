#ifndef SOCKET_COMM
#define SOCKET_COMM

int socketServerOpen(int portno);
int socketWaitForClient( int sockfd);
void socketServerClose(int fd);

void sendData( int sockfd, int x );
int getData( int sockfd );

#endif //SOCKET_COMM
