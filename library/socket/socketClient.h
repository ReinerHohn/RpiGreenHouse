#ifndef SOCKET_CLIENT
#define SOCKET_CLIENT

struct hostent* socketClientOpen(int* sockfd, char* szServerIp);
void socketClientConnect(int sockfd, struct hostent* server, int portno);

int sendData(int sockfd, char *buffer, int nLength);
int recData(int sockfd, char *buffer, int nLength);
void socketClientClose(int sockfd);

#endif //SOCKET_CLIENT
