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
socklen_t length;
FILE *fptr;
char buff[256];
char *filename = "1gb.txt";
int bytes_sent = 0;
#define SERVER_PORT 5060
#define SERVER_IP_ADDRESS "192.168.56.1";

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

    int connc = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connc == -1)
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
    length = sizeof(buff);

    //cubic
    if (getsockopt(sock, IPPROTO_IP, TCP_CONGESTION, buff, &length) != 0)
    {
        perror("getsockopt");
        return -1;
    }
    // puts(buff);
    printf("Current: %s\n", buff);
    for (int i = 0; i < 5; i++)
    {
        send_file(fptr, sock);
    }
    printf("-------- sending with cubic done------- \n");
    //reno
    strcpy(buff, "reno");
    length = strlen(buff);
    if (setsockopt(sock, IPPROTO_IP, TCP_CONGESTION, buff, length) != 0)
    {
        perror("setsockopt");
        return -1;
    }
    length = sizeof(buff);
    if (getsockopt(sock, IPPROTO_IP, TCP_CONGESTION, buff, &length) != 0)
    {
        perror("getsockopt");
        return -1;
    }
    printf("New: %s\n", buff);
    //send file
    for (int i = 0; i < 5; i++)
    {
        // bytes_sent += send(sock, fgets(buff, 256, fptr), 256, 0);
        send_file(fptr, sock);
    }
    printf("-------- sending with reno done------- \n");
    close(sock);
    return 0;
}