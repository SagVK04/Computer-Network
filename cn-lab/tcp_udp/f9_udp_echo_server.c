[f6.c] is a UDP ECHO server. The calls made by the server are shown in the above figure.
---------------------------------------------------------------------
// file-name f6.c
// UDP ECHO server using all interfaces
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERVER_PORT 50000
#define BUFFERSIZE 512 // no more than this
extern int errno;
int main( int argc, char *argv[] )
{
int ser_sd;
struct sockaddr_in server_addr, client_addr;
socklen_t clientlength;
char buffer[BUFFERSIZE];
ssize_t i, j ;
int n;
// an IPv4 datagram socket ( UDP socket ) is created
ser_sd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
if( ser_sd == -1 ) { perror("socket-call"); exit(1); }
// zero may be used for IPPROTO_UDP
// members of [server_addr] structure are initialised with the
// address of the server
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
server_addr.sin_port = htons(SERVER_PORT);
// bind socket with IPv4 address of server
n = bind( ser_sd, (struct sockaddr *) & server_addr, sizeof(server_addr) );
clientlength = (socklen_t)sizeof(client_addr);
while( 1 ) // endless loop
{
printf("%s: using UDP port-%u \n", argv[0], SERVER_PORT );
memset( buffer, '\0', BUFFERSIZE );
i = recvfrom( ser_sd, buffer, BUFFERSIZE,0, (struct sockaddr *) &client_addr, &clientlength );
if( i == -1 ) perror("recvfrom-call");
// printing client's address and port are not essential
printf("client's IP address = %s \n", inet_ntoa(client_addr.sin_addr) );
printf("client's port = %u \n", ntohs(client_addr.sin_port) );
j = write( STDOUT_FILENO, "received from client-> ", 23 );
if( j == -1 ) perror("write-stdout");
printf("%s \n", buffer );
j = sendto( ser_sd, buffer, i, 0, (struct sockaddr *) &client_addr, clientlength);
if( j == -1 ) perror("sendto-call");
} // end of while block
close( ser_sd );
exit(0);
}
/*Assignment-6:
Compile the program and start the server
T-2 $ gcc -Wall ./f6.c -o ./six
T-2 $ ./six
Test the server with [ udp-echo-client ] written by you in a previous experiment.
T-3 $ udp-echo-client 127.0.0.1 50000
T-3 $ udp-echo-client 192.168.5.___ 50000
Did the server run correctly ? ( Y / n )
End of Assignment-6.*/