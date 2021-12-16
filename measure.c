#include "stdio.h"

#if defined _WIN32
// See at https://msdn.microsoft.com/en-us/library/windows/desktop/ms740506(v=vs.85).aspx
// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>

#else //  linux
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#endif

#define SERVER_IP_ADDRESS "10.0.0.18"
#define SERVER_PORT 5060
int bytesRecv = -1;
char buff[256];
time_t time_start, time_end;

int main()
{
  int sock = -1;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1)
  {
    printf("could not create socket");
    return -1;
  }

  struct sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(SERVER_PORT); //network order

  int b = bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  if (b == -1)
  {
    printf("Could Not bind");
    return -1;
  }

  printf("Bind() success\n");
  // take the file 10 times (cubic 5 times + reno 5 times)
  int lis = listen(sock, 10);

  if (lis == -1)
  {
    printf("listen FAILED , EROR");
    return -1;
  }

  printf("Waiting for incoming TCP-connections...\n");

  struct sockaddr_in senderAddress;
  socklen_t senderAddressLen = sizeof(senderAddress);
  int senderSocket = accept(sock, (struct sockaddr *)&senderAddress, &senderAddressLen);

  if (senderSocket == -1)
  {
    printf("accpet fucntion not Working!!");
    return -1;
  }

  printf("-----first run--------");
  time_start = clock();
  do
  {

    bytesRecv = recv(senderSocket, buff, strlen(buff), 0);
  } while (bytesRecv > 0);
  time_end = clock() - time_start;
  double time_first = (double)(time_end / CLOCKS_PER_SEC);

  printf("-----second run--------");

  time_start = clock();
  do
  {

    bytesRecv = recv(senderSocket, buff, strlen(buff), 0);
  } while (bytesRecv > 0);
  double time2 = (double)(time_end / CLOCKS_PER_SEC);
  double time_second = time_end;

  printf("First: %f , Second: %f", time_first, time_second);
  close(sock);

  return 0;
}