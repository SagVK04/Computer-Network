// In [f3a.c] the values of a client's address and port number may be printed from the server. Full from of [accept] call is used.
// file-name f3.c
// TCP ECHO server listening on [ eth0 ] interface.
// prints IP address and port-number of each client
// usage: program-name
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#define SERVER_ADDR "eth0-interface-address"
#define SERVER_PORT 50000
#define BUFFERSIZE 1024
int main(int argc, char *argv[]) {
    int ser_sd;
    int tempsockfd, n ;
    socklen_t clientlength;
    ssize_t i, j ;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr, client_addr;
    ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if(ser_sd == -1) {
        perror("socket-call");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    n = inet_aton(SERVER_ADDR, &(server_addr.sin_addr));
    if(n == 0) {
        printf("inet_aton: Invalid address\n");
        exit(1);
    }
    server_addr.sin_port = htons(SERVER_PORT);
    n = bind(ser_sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(n == -1) {
        perror("bind-call");
        exit(1);
    }
    n = listen(ser_sd, 1);
    if(n == -1) {
        perror("listen-call");
        exit(1);
    }
    clientlength = sizeof(client_addr);
    while(1) {
        printf("%s : waiting for client's request on port %u \n",argv[0],SERVER_PORT);
        tempsockfd = accept(ser_sd, (struct sockaddr *) &client_addr, &clientlength);
        if(tempsockfd == -1) perror("accept-call");
        printf("client's port = %u\n", ntohs(client_addr.sin_port));
        printf("client's address = %s\n", inet_ntoa(client_addr.sin_addr));
        memset(buffer, '\0', BUFFERSIZE);
        i = read(tempsockfd, buffer, BUFFERSIZE);
        if(i == -1) {
            perror("socket-read");
            exit(1);
        }
        j = write(STDOUT_FILENO,"Received from client->", 22);
        if(j == -1) perror("screen-write");
        printf("%s\n", buffer);
        j = write(tempsockfd, buffer, i);
        if(j == -1) {
            perror("socket-write");
            exit(1);
        }
        n = shutdown(tempsockfd,SHUT_RDWR);
        if(n == -1) {
            perror("shutdown");
            exit(1);
        }
        close(tempsockfd);
    }
}
// end of main
/*Assignment-3:
Define server's address correctly, compile and run the server
T-2 $ gcc -Wall ./f3.c -o ./three
T-2 $ ./three
Request a friend ( IP-address = ) to test your sever
friend $ tcp-echo-client 192.168.5.___ ___________
Before entering a string, friend finds out his ephemeral port for this connection.
friend in another terminal T-5 # netstat -atn
friend's ephemeral port -->
Record from your server's output
client's port--------->
client's address------>
Terminate server with CTRL-C
T-2 CTRL-C
T-2 $
End of Assignment-3.*/