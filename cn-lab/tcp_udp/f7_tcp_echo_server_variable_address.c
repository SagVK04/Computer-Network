// file-name f4.c
// TCP ECHO server listening on all interfaces.
// prints client's IP address and port-number
// usage: program-name
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#define SERVER_PORT 50000
#define BUFFERSIZE 1024
int main( int argc, char *argv[] )
{
int ser_sd;
int tempsockfd, n ;
socklen_t clientlength;
ssize_t i, j ;
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr, client_addr;
ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( ser_sd == -1 ) { perror("socket-call"); exit(1); }
server_addr.sin_family = AF_INET;
/*
If INADDR_ANY is specified in the bind call the socket will be bound to all local interfaces.
*/
server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
server_addr.sin_port = htons(SERVER_PORT);
n = bind( ser_sd, (struct sockaddr *) & server_addr, sizeof(server_addr) );
if( n == -1) { perror("bind-call"); exit(1); }
n = listen( ser_sd, 1 );
if( n == -1 ) { perror("listen-call"); exit(1); }
clientlength = sizeof(client_addr);
while ( 1 ) // endless loop
{
printf( "%s : waiting for client's request on port %u \n", argv[0], SERVER_PORT );
tempsockfd = accept( ser_sd, (struct sockaddr *) &client_addr, &clientlength );
if( tempsockfd == -1 ) perror("accept-call");
// IP address and port of a client is printed
printf("client's port = %u \n", ntohs( client_addr.sin_port ) );
printf("client's = %s \n", inet_ntoa(client_addr.sin_addr) );
// clear buffer before reading into it
memset( buffer, '\0', BUFFERSIZE );
i = read( tempsockfd, buffer, BUFFERSIZE );
if( i == -1 )
{
perror("socket-read");
exit(1);
}
j = write( STDOUT_FILENO,"Received from client->", 22 );
if( j == -1 ) perror("screen-write");
printf("%s \n", buffer );
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
} // end of main
/*Assignment-4:
Compile and run the server
T-2 $ gcc -Wall ./f4.c -o ./four
T-2 $ ./four
From another terminal test the server using [eth0] interface
T-3 $ tcp-echo-client 192.168.5.___ _____
Did the server run correctly ? ( Y / n )
From another terminal test the server using loopback interface
T-3 $ tcp-echo-client 127.0.0.1 _____
Did the server run correctly ? ( Y / n )
Stop server with CTRL-C and restart it
T-2 $ ./four
Use the following command and record output.
T-5 # netstat -atnp | grep four
Proto Local Address Foreign Address State PID/Program name
tcp LISTEN _____/four
From another terminal connect to the server
T-3 $ tcp-echo-client 192.168.5.____ ______
Do not enter any string now...
Use [netstat] command and record output. A typical output is shown
T-5 # netstat -atnp | grep four
Proto Local Address Foreign Address State PID/name
tcp 0 0.0.0.0:_____ 0.0.0.0:* LISTEN ____/four
tcp 192.168.5.___:____ 192.168.5.___:____ ESTABLISHED ____/four
Enter a string in the client program.
Test the server again
T-3 $ tcp-echo-client 127.0.0.1 ______
Do not enter any string now...
Use [netstat] command and record output. A typical output is shown
T-5 # netstat -atnp | grep four
Proto Local Address Foreign Address State PID/name
tcp 0 0.0.0.0:_____ 0.0.0.0:* LISTEN four
tcp 127.0.0.1:_____ 127.0.0.1:________ ESTABLISHED four
Run a TCP port scan on [eth0] interface for the server's port
End of Assignment-4.
*/