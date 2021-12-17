
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

#define SERVER_PORT 5060
int main(int argc, char **argv)
{

  FILE *fptr;
  int cubic_bytes_rec, reno_bytes_rec;
  char cubic_buff[256], reno_buff[256];
  int sock;
  int cubic_accp, reno_accp;
  int b;
  int lis;
  struct sockaddr_in cubic_server_addr, reno_server_addr;
  socklen_t cubic_len, reno_len;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    printf("could not create socket!!");
    return -1;
  }

  memset(&cubic_server_addr, 0, sizeof(cubic_server_addr));
  cubic_server_addr.sin_family = AF_INET;
  cubic_server_addr.sin_port = htons(SERVER_PORT);
  cubic_server_addr.sin_addr.s_addr = INADDR_ANY;

  b = bind(sock, (struct sockaddr *)&cubic_server_addr, sizeof(cubic_server_addr));

  if (b == -1)
  {
    printf("could not bind!!");
    return -1;
  }

  lis = listen(sock, 10);
  if (lis == -1)
  {
    printf("listen FAILED , EROR!!");
    return -1;
  }

  printf("\nWaiting for incoming TCP-connections...\n");

  cubic_accp = accept(sock, (struct sockaddr *)&cubic_server_addr, &cubic_len);
  if (cubic_accp == -1)
  {
    printf("accept FAILED , EROR!!");
    return -1;
  }

  clock_t start_t, end_t;
  double total_t;
  start_t = clock();
  printf("\ntime started at: %f ,  with cubic\n", (double)start_t / CLOCKS_PER_SEC);
  do
  {
    cubic_bytes_rec = recv(cubic_accp, cubic_buff, 256, 0);

  } while (cubic_bytes_rec > 0);
  end_t = clock();

  printf("\ntime ended at: %f ,  with cubic\n", (double)end_t / CLOCKS_PER_SEC);
  double time_first = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("\ntotal time cubic: %f \n", time_first);
  close(cubic_accp);
  printf("\n----------SWITCHING ALGOS-----------\n");

  reno_accp = accept(sock, (struct sockaddr *)&reno_server_addr, &reno_len);
  if (reno_accp == -1)
  {
    printf("accept FAILED , EROR!!");
    return -1;
  }

  start_t = clock();
  printf("\ntime started at: %f ,  with reno\n", (double)start_t / CLOCKS_PER_SEC);
  do
  {
    reno_bytes_rec = recv(reno_accp, reno_buff, 256, 0);

  } while (reno_bytes_rec > 0);

  end_t = clock();
  printf("\ntime ended at: %f ,  with reno\n", (double)end_t / CLOCKS_PER_SEC);
  time_first = (double)(end_t - start_t) / CLOCKS_PER_SEC;
  printf("\ntotal time reno: %f \n", time_first);
  close(sock);
}