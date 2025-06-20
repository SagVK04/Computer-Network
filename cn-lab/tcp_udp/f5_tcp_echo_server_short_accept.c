In the previous programs the variable [ client_addr ] was used as an argument of [accept] call. [accept] call may not use this variable. In [f2.c], a short form of [ accept ] call is used. In short form of the [ accept ] call, the variable [clientlength] is also not needed.
// file-name f2.c
// TCP ECHO server listening on [ eth0 ] interface.
// short form of [accept] call is used.
// usage: program-name
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#define SERVER_ADDRESS "eth0-interface-address"
#define SERVER_PORT 50000
#define BUFFERSIZE 1024
int main( int argc, char *argv[] )
{
int ser_sd;
int tempsockfd, n ;
ssize_t i, j ;
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr;
ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( ser_sd == -1 ) { perror("socket-call"); exit(1); }
server_addr.sin_family = AF_INET;
n = inet_aton( SERVER_ADDRESS, &(server_addr.sin_addr) );
if( n == 0 ) { printf("Invalid address\n"); exit(1); }
server_addr.sin_port = htons(SERVER_PORT);
n = bind( ser_sd, (struct sockaddr *) & server_addr, sizeof(server_addr) );
if( n == -1) { perror("bind-call"); exit(1); }
n = listen( ser_sd, 1 );
if( n == -1 ) { perror("listen-call"); exit(1); }
while ( 1 ) // endless loop
{
printf( "%s : waiting for client's request on port %u \n", argv[0], SERVER_PORT );
// note the 2nd and 3rd arguments
tempsockfd = accept( ser_sd, NULL, 0 );
if( tempsockfd == -1 ) perror("accept-call");
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
} // end of while block
} // end of main
/*Assignment-2:
Define server's address correctly, compile and start the server
T-2 $ gcc -Wall ./f2.c -o ./two
T-2 $ ./two
From another terminal test the server
T-3 $ tcp-echo-client 192.168.5.___ 50000
Did the server run correctly ? ( Y / n )
End of Assignment-2.*/