#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <bits/types/FILE.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <time.h>

char buff[256] = {0};
int bytes_rec;
int accp;
socklen_t len;

#define SERVER_PORT 5060
int main(int argc, char **argv)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    printf("could not create socket!!");
    return -1;
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  int b = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

  if (b == -1)
  {
    printf("could not bind!!");
    return -1;
  }

  printf("Bind() success\n");
  int lis = listen(sock, 10);
  if (lis == -1)
  {
    printf("listen FAILED , EROR!!");
    return -1;
  }
  printf("listen() success\n");

  printf("Waiting for incoming TCP-connections...\n");

  accp = accept(sock, (struct sockaddr *)&server_addr, &len);
  if (accp == -1)
  {
    printf("accept FAILED , EROR!!");
    return -1;
  }

  printf("accept() success\n");
  int count = 4114 / 2;
  clock_t start_t, end_t;
  double total_t;
  start_t = clock();
  printf("\ntime started at: %f ,  with cubic\n", (double)start_t / CLOCKS_PER_SEC);

  do
  {
    bytes_rec = recv(accp, buff, 256, 0);
    count--;
  } while (count > 0);
  end_t = clock();
  printf("\ntime ended at: %f ,  with cubic\n", (double)end_t / CLOCKS_PER_SEC);
  double time_first = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("\ntotal time cubic: %f \n", time_first);
  printf("\n----------SWITCHING ALGOS-----------\n");
  start_t = clock();
  printf("\ntime started at: %f ,  with reno\n", (double)start_t / CLOCKS_PER_SEC);
  do
  {
    bytes_rec = recv(accp, buff, 256, 0);

  } while (bytes_rec > 0);

  end_t = clock();
  printf("\ntime ended at: %f ,  with reno\n", (double)end_t / CLOCKS_PER_SEC);

  double time_second = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("\ntotal time reno: %f \n", time_second);
  total_t = time_first + time_second;

  printf("\ntime finished at: %f ,  for both CC algos\n", total_t);

  return 0;
}