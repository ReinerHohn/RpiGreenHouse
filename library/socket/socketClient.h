#ifndef SOCKET_CLIENT
#define SOCKET_CLIENT

struct hostent* socketClientOpen(int* sockfd, char* szServerIp);
void socketClientConnect(int sockfd, struct hostent* server, int portno);

int socketClientSendData(int sockfd, char *buffer, int nLength);
int socketClientGetData(int sockfd, char *buffer, int nLength);
void socketClientClose(int sockfd);

#endif //SOCKET_CLIENT
