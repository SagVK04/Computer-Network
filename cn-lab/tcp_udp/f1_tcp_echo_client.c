//file-name f1.c TCP echo client
// usage -> program-name server-address server-port
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFERSIZE 1024 // arbitrary
//#define CHECK
int main(int argc, char *argv[])
{
int sd; // used as socket descriptor
ssize_t i,j;
int n; // used to check returned value
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr; // An IPv4 address
if( argc != 3 )
{
printf("Usage: %s server-address server-port \n", argv[0] );
printf("Example: %s 192.168.5.148 12345 \n", argv[0] );
exit(1);
}
printf( "TCP-echo-client starting...\n" );
// A TCP ( IPv4 stream ) socket is created.
sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( sd == -1 ) { perror("socket-call"); exit( 1 ); }
/*
The descriptor [sd] is not useful now. This is to be connected to server's address with [connect() ] system call. Before making this call, the members of structure [ server_addr ] are to be initialized.
An IP socket address is defined as a combination of an IP interface address and a port number, using a protocol.
IPv4 address is a structure [ struct sockaddr_in ]
The structure has the following members:
struct sockaddr_in {
sa_family_t sin_family; // address family: AF_INET
u_int16_t sin_port; // port in network byte order
struct in_addr sin_addr; // Internet address
};
Internet address:
struct in_addr {
u_int32_t s_addr; // address in network byte order
};
You may read manual [ ip(7) ].
The variable [server_addr], used by you, is an instance of the structure [ struct sockaddr_in ]
*/
// First member of structure [ server_addr ] is initialized with the address family consistent with the //protocol family of the socket.
server_addr.sin_family = AF_INET;
/*
The port number is to be converted from host byte order to network byte order. Network byte order is big endian, whereas host byte order may be little endian or big endian. In your PC ( i80x86 ) host byte order is little endian. The function [ htons() ] is used to convert port number from host byte order to network byte order. Even if the host byte order is big endian, calling [htons()] is not a mistake. You may read manual [ htons(3) ].
argv[2] is the port number in our program.
*/
// Second member of structure [server_addr] is initialised
server_addr.sin_port = htons( atoi(argv[2]) );
/*
Internet host address given as standard numbers-and-dots notation is to be converted into binary data and is to be stored as the [ server_addr.sin_addr ] of [ server_addr ] structure. The function [inet_aton()] is used to do that. This function returns nonzero if the address is valid, zero if not. You may read manual [ inet_aton(3) ].
*/
// Third member of structure [server_addr] is initialised.
// argv[1] is the server address in dotted decimal quad
n = inet_aton( argv[1], &(server_addr.sin_addr) );
if( n == 0 ) { printf("inet_aton-Invalid address\n"); exit(1); }
n = connect( sd, (struct sockaddr *) &server_addr, sizeof(server_addr) );
if( n == -1 ) { perror("connect-call"); exit(1); }
// the descriptor [sd] is now connected to server's socket
#ifdef//file-name f1.c TCP echo client
// usage -> program-name server-address server-port
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFERSIZE 1024 // arbitrary
//#define CHECK
int main(int argc, char *argv[])
{
int sd; // used as socket descriptor
ssize_t i,j;
int n; // used to check returned value
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr; // An IPv4 address
if( argc != 3 )
{
printf("Usage: %s server-address server-port \n", argv[0] );
printf("Example: %s 192.168.5.148 12345 \n", argv[0] );
exit(1);
}
printf( "TCP-echo-client starting...\n" );
// A TCP ( IPv4 stream ) socket is created.
sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( sd == -1 ) { perror("socket-call"); exit( 1 ); }
/*
The descriptor [sd] is not useful now. This is to be connected to server's address with [connect() ] system call. Before making this call, the members of structure [ server_addr ] are to be initialized.
An IP socket address is defined as a combination of an IP interface address and a port number, using a protocol.
IPv4 address is a structure [ struct sockaddr_in ]
The structure has the following members:
struct sockaddr_in {
sa_family_t sin_family; // address family: AF_INET
u_int16_t sin_port; // port in network byte order
struct in_addr sin_addr; // Internet address
};
Internet address:
struct in_addr {
u_int32_t s_addr; // address in network byte order
};
You may read manual [ ip(7) ].
The variable [server_addr], used by you, is an instance of the structure [ struct sockaddr_in ]
*/
// First member of structure [ server_addr ] is initialized with the address family consistent with the //protocol family of the socket.
server_addr.sin_family = AF_INET;
/*
The port number is to be converted from host byte order to network byte order. Network byte order is big endian, whereas host byte order may be little endian or big endian. In your PC ( i80x86 ) host byte order is little endian. The function [ htons() ] is used to convert port number from host byte order to network byte order. Even if the host byte order is big endian, calling [htons()] is not a mistake. You may read manual [ htons(3) ].
argv[2] is the port number in our program.
*/
// Second member of structure [server_addr] is initialised
server_addr.sin_port = htons( atoi(argv[2]) );
/*
Internet host address given as standard numbers-and-dots notation is to be converted into binary data and is to be stored as the [ server_addr.sin_addr ] of [ server_addr ] structure. The function [inet_aton()] is used to do that. This function returns nonzero if the address is valid, zero if not. You may read manual [ inet_aton(3) ].
*/
// Third member of structure [server_addr] is initialised.
// argv[1] is the server address in dotted decimal quad
n = inet_aton( argv[1], &(server_addr.sin_addr) );
if( n == 0 ) { printf("inet_aton-Invalid address\n"); exit(1); }
n = connect( sd, (struct sockaddr *) &server_addr, sizeof(server_addr) );
if( n == -1 ) { perror("connect-call"); exit(1); }
// the descriptor [sd] is now connected to server's socket
#ifdef//file-name f1.c TCP echo client
// usage -> program-name server-address server-port
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFERSIZE 1024 // arbitrary
//#define CHECK
int main(int argc, char *argv[])
{
int sd; // used as socket descriptor
ssize_t i,j;
int n; // used to check returned value
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr; // An IPv4 address
if( argc != 3 )
{
printf("Usage: %s server-address server-port \n", argv[0] );
printf("Example: %s 192.168.5.148 12345 \n", argv[0] );
exit(1);
}
printf( "TCP-echo-client starting...\n" );
// A TCP ( IPv4 stream ) socket is created.
sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( sd == -1 ) { perror("socket-call"); exit( 1 ); }
/*
The descriptor [sd] is not useful now. This is to be connected to server's address with [connect() ] system call. Before making this call, the members of structure [ server_addr ] are to be initialized.
An IP socket address is defined as a combination of an IP interface address and a port number, using a protocol.
IPv4 address is a structure [ struct sockaddr_in ]
The structure has the following members:
struct sockaddr_in {
sa_family_t sin_family; // address family: AF_INET
u_int16_t sin_port; // port in network byte order
struct in_addr sin_addr; // Internet address
};
Internet address:
struct in_addr {
u_int32_t s_addr; // address in network byte order
};
You may read manual [ ip(7) ].
The variable [server_addr], used by you, is an instance of the structure [ struct sockaddr_in ]
*/
// First member of structure [ server_addr ] is initialized with the address family consistent with the //protocol family of the socket.
server_addr.sin_family = AF_INET;
/*
The port number is to be converted from host byte order to network byte order. Network byte order is big endian, whereas host byte order may be little endian or big endian. In your PC ( i80x86 ) host byte order is little endian. The function [ htons() ] is used to convert port number from host byte order to network byte order. Even if the host byte order is big endian, calling [htons()] is not a mistake. You may read manual [ htons(3) ].
argv[2] is the port number in our program.
*/
// Second member of structure [server_addr] is initialised
server_addr.sin_port = htons( atoi(argv[2]) );
/*
Internet host address given as standard numbers-and-dots notation is to be converted into binary data and is to be stored as the [ server_addr.sin_addr ] of [ server_addr ] structure. The function [inet_aton()] is used to do that. This function returns nonzero if the address is valid, zero if not. You may read manual [ inet_aton(3) ].
*/
// Third member of structure [server_addr] is initialised.
// argv[1] is the server address in dotted decimal quad
n = inet_aton( argv[1], &(server_addr.sin_addr) );
if( n == 0 ) { printf("inet_aton-Invalid address\n"); exit(1); }
n = connect( sd, (struct sockaddr *) &server_addr, sizeof(server_addr) );
if( n == -1 ) { perror("connect-call"); exit(1); }
// the descriptor [sd] is now connected to server's socket
#ifdefCHECK
printf("Check with [ # netstat -tn ] in another terminal\n" );
printf("Check TCP connection establishment\n" );
while(1) { sleep(1); } // endless loop
#endif
/*
[bind()] system call was not called on [sd] before [ connect() ] system call. As [connect()] was called on a unbound socket, [sd] was automatically bound to a random free port ( ephemeral port ) with the local address set to [INADDR_ANY].
*/
write( STDOUT_FILENO, "Enter the string:", 17 );
// clear buffer before reading
memset( buffer, '\0', BUFFERSIZE );
i = read( STDIN_FILENO, buffer, BUFFERSIZE );
if( i == -1 ) { perror("read1"); exit(1); }
printf( "bytes read from keyboard=%u\n", i );
// write contents of buffer on server's socket
j = write( sd, buffer, i );
if( j == -1 ) { perror("write1"); exit(1); }
printf("bytes written in server's socket=%u\n",j);
// clear buffer before reading
memset( buffer, '\0', BUFFERSIZE );
// read from server's socket into buffer
i = read( sd, buffer, BUFFERSIZE );
if( i == -1 ) { perror("read2"); exit(1); }
printf("bytes read from server's socket=%u\n", i );
write( STDOUT_FILENO,"Reply from echo server->", 24 );
j = write( STDOUT_FILENO, buffer, i );
if( j == -1 ) { perror("write2"); exit(1); }
// Shutdown the both-way ( duplex ) connection.
shutdown(sd, SHUT_RDWR);
return 0;
}
/*$ gcc -Wall ./f1.c -o ./one-a
Run the program using wrong number of arguments to see usage message
$ ./one-a
$ ./one-a 192.168.5.251
Run the program correctly. Replace 192.168.5.251 with a suitable server address
$ ./one-a 192.168.5.251 7*/