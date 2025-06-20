In [f3.c], server address and server port number, are supplied as two arguments to the program.
----------------------------------------------------------------------
// file-name f3.c UDP echo client
// usage -> program-name server-address server-port
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE 512
// Uncomment the following line to use [recvfrom()] call in full form.
//#define RECVFROM_FULL
// Uncomment the following line to use [recvfrom()] call in stripped form
//#define RECVFROM_STRIPPED
// Uncomment the following line to use [recv()] call
//#define RECV
// Uncomment the following line to use [read()] call
//#define READ
// Uncomment the following line to print numeric values of some symbolic constants
//#define DEBUG
int main( int argc, char *argv[])
{
int sd; // socket descriptor
int n; // to check returned value of function calls.
ssize_t i;
char buffer[SIZE];
struct sockaddr_in server_addr;
struct sockaddr_in source_addr;
#ifdef RECVFROM_FULL
// following variables are used to illustrate use of [ recvfrom ] call in full form
socklen_t source_addr_length;
unsigned long nbo; // host address in network byte order
unsigned long hbo; // host address in network byte order
char *host_addr;
#endif
if( argc != 3 )
{
printf("usage -> prog-name server-address server-port \n");
printf("example -> %s 172.16.2.15 7 \n", argv[0] );
exit(1);
}
printf("udp-echo-client: PID of the program = %u\n", getpid() );
// an IPv4 datagram socket ( UDP socket ) is created
sd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
// zero may be used for IPPROTO_UDP
if( sd == -1 ) { perror("socket-call"); exit(1); }
// Initialise the members of server address
server_addr.sin_family = AF_INET;
n = inet_aton( argv[1], &(server_addr.sin_addr) );
if( n == 0 ) { perror("invalid-address"); exit(1); }
server_addr.sin_port = htons( atoi(argv[2]) );
write( STDOUT_FILENO, "u-e-c: Enter the string:", 24 );
memset( buffer, '\0', SIZE );
i = read( STDIN_FILENO, buffer, SIZE );
n = sendto( sd, buffer, i, 0, ( struct sockaddr *) &server_addr, sizeof( server_addr) );
if( n == -1 ) { perror("sendto-call"); exit(1); }
printf("u-e-c: Sent %u bytes to server \n", n );
// clearthe buffer, before receiving from server
memset( buffer, '\0', SIZE );
#ifdef RECVFROM_FULL
printf("u-e-c: Using recvfrom() call in full form \n");
source_addr_length = sizeof(source_addr);
n = recvfrom( sd, buffer,SIZE, MSG_PEEK, (struct sockaddr *) &source_addr, &source_addr_length );
if( n == -1 ) { perror("recvfrom-call-full"); exit(1); }
// Print the value of a symbolic constant we are interested in
printf("recvfrom-full: AF_INET = %u \n", AF_INET);
/* [recvfrom()] system call has filled in members of the variable [source_addr]. The values of the members are now retrieved. */
printf("rf-full: source address family = %u\n", source_addr.sin_family );
nbo = source_addr.sin_addr.s_addr;
printf("rf-full: source address in network byte order ");
printf("= %lX Hex \n", nbo );
hbo = ntohl(source_addr.sin_addr.s_addr);
printf("rf-full: source address in host byte order = %lX Hex \n", hbo );
host_addr = inet_ntoa(source_addr.sin_addr);
printf("rf-full: source address in dotted decimal quad = %s \n", host_addr );
printf("rf-full: source port in network byte order = %X Hex \n", source_addr.sin_port );
printf("rf-full: source port in host byte order = %X Hex\n", ntohs(source_addr.sin_port) );
printf("rf-full: source address length = %u bytes\n", source_addr_length );
#endif
#ifdef RECVFROM_STRIPPED
printf("u-e-c: Using recvfrom() call in stripped form \n");
//We are not interested in source address
n = recvfrom( sd, buffer,SIZE, 0, NULL, 0 );
if( n == -1 ) { perror("recvfrom-call-stripped"); exit(1); }
#endif
#ifdefRECV
printf("u-e-c: Using recv() call \n");
// We are not interested in source address but want to use [flag]
n = recv( sd, buffer, SIZE, MSG_PEEK );
if( n == -1 ) { perror("recvfrom-call-stripped"); exit(1); }
#endif
#ifdef READ
printf("u-e-c: Using read() call \n");
// We are not interested in source address and don't want to use [flag]
n = read( sd, buffer, SIZE);
if( n == -1 ) { perror("read-call"); exit(1); }
#endif
printf("u-e-c: Received %u bytes from server \n", n );
write( STDOUT_FILENO, "u-e-c: From server->", 20 );
write( STDOUT_FILENO, buffer, n );
shutdown( sd, SHUT_RDWR );
return 0;
}
----------------------------------------------------------------------

/*Assignment-3:
(a) Define only READ and compile
$ gcc -Wall ./f3.c -o ./three-a
Connect to any UDP [echo] server
$ ./three-a 192.168.5.251 7
Did the program run correctly? (Y / n )
(b) Undefine READ, define only RECV and compile
$ gcc -Wall ./f3.c -o ./three-b
Connect to any UDP [echo] server
$ ./three-b 192.168.5.149 7
Did the program run correctly? (Y / n )
(c) Undefine RECV, define only RECVFROM_STRIPPED and compile
$ gcc -Wall ./f3.c -o ./three-c
Connect to any UDP [echo] server
$ ./three-c 192.168.5.161 7
Did the program run correctly ? ( Y / n )
(d) Comment RECVFROM_STRIPPED, define only RECVFROM_FULL and compile
$ gcc -Wall ./f3.c -o ./three-d
Connect to any [echo] server using UDP
$ ./three-d ___.___.___.___ 7
Did the program run correctly? ( Y / n )
Record the following...
Value of symbolic constant AF_INET =
source protocol family =
source address in network byte order = Hex
source address in host byte order = Hex
source address in dotted decimal quad =
source port in network byte order = Hex
source port in host byte order = Hex
source address length = bytes
End of Assignment-3.*/