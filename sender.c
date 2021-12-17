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


char *filename = "1gb.txt";
#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "192.168.56.1";

//sending file
void send_file(FILE *fp, int sock)
{
    char data[256] = {0};

    while (fgets(data, 256, fp) != NULL)
    {
        int s = send(sock, data, sizeof(data), 0);

        if (s == -1)
        {
            printf("send fucntion not Working!!!");
            exit(1);
        }
        bzero(data, 256);
    }
}
// to open diff sockets
int openingSock(int sock, char *buff, int conn, int bytes_sent, struct sockaddr_in server_addr, socklen_t len)
{
    FILE *fptr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("could not create socket!!");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    conn = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (conn == -1)
    {
        printf("could not make Connection");
        return -1;
    }

    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("could not read file");
        exit(1);
    }
    len = sizeof(buff);

    for (int i = 0; i < 5; i++)
    {
        send_file(fptr, sock);
    }
    strcpy(buff, "reno");
    len = strlen(buff);
//switching CC algo to reno
    if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, buff, len) != 0)
    {
        perror("setsockopt");
        return -1;
    }

    close(sock);
}

int main(int argc, char **argv)
{
    FILE *fptr;
    socklen_t len;
    int cubic_bytes_sent = 0, reno_bytes_sent = 0;
    // char cubic_buff[256], reno_buff[256];
    char buff[256];
    int cubic_sock, reno_sock;
    int cubic_conn, reno_conn;
    struct sockaddr_in cubic_server_addr, reno_server_addr;
    len = sizeof(buff);
    printf("Current: cubic\n");
    openingSock(cubic_sock, buff, cubic_conn, cubic_bytes_sent, cubic_server_addr, len);
    openingSock(reno_sock, buff, reno_conn, reno_bytes_sent, reno_server_addr, len);
    printf("New: %s\n", buff);
}
