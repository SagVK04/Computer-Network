#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ETH0 "your ip address"
#define SERVER_PORT 50001
#define BUFFERSIZE 1024

int main(int argc, char *argv[])
{
    int ser_sd;
    int tempsockfd, n;
    socklen_t clientlength;
    ssize_t i, j;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr, client_addr;

    if (argc != 1)
    {
        printf("usage: %s \n", argv[0]);
        exit(1);
    }

    ser_sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ser_sd == -1) { perror("socket-call"); exit(1); }

    server_addr.sin_family = AF_INET;
    n = inet_aton(ETH0, &(server_addr.sin_addr));
    if (n == 0) { printf("inet_aton: Invalid address\n"); exit(1); }

    server_addr.sin_port = htons(SERVER_PORT);

    n = bind(ser_sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (n == -1) { perror("bind-call"); exit(1); }

    n = listen(ser_sd, 1);
    if (n == -1) { perror("listen-call"); exit(1); }

    clientlength = sizeof(client_addr);

    while (1)
    {
        printf("%s : waiting for client's request on port %u \n", argv[0], SERVER_PORT);

        tempsockfd = accept(ser_sd, (struct sockaddr *)&client_addr, &clientlength);
        if (tempsockfd == -1) { perror("accept-call"); }

        memset(buffer, '\0', BUFFERSIZE);

        i = read(tempsockfd, buffer, BUFFERSIZE);
        if (i == -1) { perror("socket-read"); exit(1); }

        j = write(STDOUT_FILENO, "Received from client->", 22);
        if (j == -1) { perror("screen-write"); exit(1); }

        printf("%s\n", buffer);

        j = write(tempsockfd, buffer, i);
        if (j == -1) { perror("socket-write"); exit(1); }

        n = shutdown(tempsockfd, SHUT_RDWR);
        if (n == -1) { perror("shutdown"); exit(1); }

        close(tempsockfd);
    }
}

