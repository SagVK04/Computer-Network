#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define SERVER_ADDRESS "eth0-interface-address"
#define SERVER_PORT 50000
#define BUFFERSIZE 1024

int main(int argc, char *argv[])
{
    int ser_sd;
    int tempsockfd, n;
    size_t i, j;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr;

    ser_sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( ser_sd == -1 ) { perror("socket-call"); exit(1); }

    server_addr.sin_family = AF_INET;
    n = inet_aton(SERVER_ADDRESS, &(server_addr.sin_addr));
    if( n == 0 ) { printf("Invalid address\n"); exit(1); }
    server_addr.sin_port = htons(SERVER_PORT);

    n = bind(ser_sd, (struct sockaddr *)& server_addr, sizeof(server_addr));
    if( n == -1 ) { perror("bind-call"); exit(1); }

    n = listen( ser_sd, 1 );
    if( n == -1 ) { perror("listen-call"); exit(1); }

    while ( 1 )
    {
        printf( "%s : waiting for client's request on port %u \n", argv[0], SERVER_PORT );

        tempsockfd = accept( ser_sd, NULL, 0 );
        if( tempsockfd == -1 ) { perror("accept-call"); }

        memset( buffer, '0', BUFFERSIZE );
        i = read( tempsockfd, buffer, BUFFERSIZE );
        if( i == -1 )
        {
            perror("socket-read");
            exit(1);
        }

        j = write( STDOUT_FILENO,"Received from client->", 22 );
        if( j == -1 ) perror("screen-write");

        printf("%s\n", buffer );

        j = write( tempsockfd, buffer, i );
        if( j == -1 )
        {
            perror("socket-write");
            exit(1);
        }

        n = shutdown(tempsockfd,SHUT_RDWR);
        if ( n == -1 )
        {
            perror("shutdown");
            exit(1);
        }

        close(tempsockfd);
    }

}

