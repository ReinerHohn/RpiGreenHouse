#ifndef SOCKET_CLIENT
#define SOCKET_CLIENT

struct hostent* socketClientOpen(int* sockfd, char* szServerIp);
void socketClientConnect(int sockfd, struct hostent* server, int portno);

void sendData(int sockfd, int x);
int getData(int sockfd);
void socketClientClose(int sockfd);

#endif //SOCKET_CLIENT
