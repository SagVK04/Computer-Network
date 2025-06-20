The returned value of [ accept() ] call creates a new socket. This new socket is used for communication with the client. Save the following as [ f1a.c ]. This program has limitations.
// file-name f1a.c
// TCP ECHO server listening on only [ eth0 ] interface
// usage: program-name
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#define ETH0 "address-of-your-eth0-interface"
// next line is an example
// #define ETH0 "192.168.5.167"
#define SERVER_PORT 50001 // an unused port in your HOST
/* Choose any port number between 50000 and 50030. This range of ports has no special significance. This range of ports are scanned in this experiment to check on servers running on different hosts.*/
#define BUFFERSIZE 1024 // arbitrary
int main( int argc, char *argv[] )
{
int ser_sd;
// [ser_sd] would be used as server's socket file descriptor
int tempsockfd, n ;
//int set_it=1;
socklen_t clientlength;
ssize_t i, j ;
char buffer[BUFFERSIZE];
struct sockaddr_in server_addr, client_addr;
// the program takes no argument
if( argc != 1 )
{
printf("usage: %s \n", argv[0] );
exit(1);
}
// a TCP socket( IPv4 stream socket ) is created
ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( ser_sd == -1 ) { perror("socket-call"); exit(1); }
// zero may be used for IPPROTO_TCP
/*
// Use this call to reuse address.
// This call has 5 arguments
n = setsockopt( ser_sd,
SOL_SOCKET,
SO_REUSEADDR,
&set_it,
sizeof(int)
);
if( n == -1 ) { perror("setsockopt"); exit(1); }
*/
// Initialise the members of the local address (server's address)
// address-family of server's address
server_addr.sin_family = AF_INET;
// local-address ( server's IP address )
n = inet_aton( ETH0, &(server_addr.sin_addr) );
if( n == 0 ) { printf("inet_aton: Invalid address\n"); exit(1); }
// local-port ( server's port )
server_addr.sin_port = htons(SERVER_PORT);
// bind the server's address with the created socket.
// [bind] call has three arguments
n = bind( ser_sd, (struct sockaddr *) & server_addr, sizeof(server_addr) );
if( n == -1) { perror("bind-call"); exit(1); }
// putting the socket in listening state
n = listen( ser_sd, 1 );
if( n == -1 ) { perror("listen-call"); exit(1); }
clientlength = sizeof(client_addr);
// [clientlength] will be used as third argument of [accept] call
while ( 1 ) // endless loop
{
printf( "%s : waiting for client's request on port %u \n", argv[0], SERVER_PORT );
// [accept()] call has three arguments
tempsockfd = accept( ser_sd, (struct sockaddr *)&client_addr, &clientlength );
if( tempsockfd == -1 ) { perror("accept-call"); }
// [tempsockfd] is used for communication with the current client
// clear buffer before reading into it
memset( buffer, '\0', BUFFERSIZE );
// read client's request in the buffer
i = read( tempsockfd, buffer, BUFFERSIZE );
if( i == -1 ) { perror("socket-read"); exit(1); }
j = write( STDOUT_FILENO,"Received from client->", 22 );
if( j == -1 ) { perror("screen-write"); exit(1); }
printf("%s \n", buffer );
// write contents of buffer to client's socket
j = write( tempsockfd, buffer, i );
if( j == -1 ) { perror("socket-write"); exit(1); }
// shutdown communication channel with this client
n = shutdown(tempsockfd,SHUT_RDWR);
if ( n == -1 ) { perror("shutdown"); exit(1); }
// remove this useless socket
close(tempsockfd);
} // endless-loop block
// program would not reach here due to endless-loop
// shutdown(ser_sd,SHUT_RDWR);
// close(ser_sd);
// exit(0);
} // end of main

/*
In the above program the server's IP address is hard coded. If this program is compiled in another host, that host's IP address is to be used in the program. This is restrictive, as the program would not run correctly in another host, without modification. So this program is not proper. The limitations of this program are to be examined.
Assignment-1:
Define the ETH0 correctly, compile [f1a.c] and run the executable
T-2 $ gcc -Wall ./f1a.c -o ./one-a
T-2 $ ./one-a
Server listening on port ______________
Verify the server's port with
T-5 # netstat -atnp | grep one-a
Was the server listening on eth0 interface? ( Y / n )
Was the server listening on loopback interface? ( y / N )
Terminate the server with CTRL-C
T-2 CTRL-C
Login in your account in terminal four and test the server. Use the program [ tcp-echo-client ], written by you in a previous experiment.
T-4 $ tcp-echo-client 192.168.5.___ port-number
Did the [echo] server run correctly? ( Y / n )
Request your friend to connect to your TCP [echo] server
friend $ tcp-echo-client your-ip-address your-port-number
Request your friend to enter a string.
Was your friend able to communicate with your [echo] server? ( Y / n )
This is an iterative server. This server can process a single client's request at a time.
Start the server
T-2 $ ./one-a
From Terminal-3, run the client program
T-3 $ tcp-echo-client 192.168.5.___ _________
Do not enter a string
From Terminal-4, run the client program
T-4 $ tcp-echo-client 192.168.5.___ _________
Enter an input string.
Did you get the echo? ( y / N )
Enter an input string in terminal-3
Did you get the echo? ( Y / n )
Go back to terminal-4
Did you get the echo? ( Y / n )
Terminate server with CTRL-C.
Use any port under 1024, as the server's port. You might define the port in [f1a.c], as shown below
#define SERVER_PORT 1023
Save the modified file as [f1b.c].
Compile...
T-2 $ gcc -Wall ./f1b.c -o ./one-b
Try to start the server as an ordinary user.
T-2 $ ./one-b
Was an ordinary user able to start the server ? ( y / N )
Start the server as [root] user, from [socket3] directory
T-2 $ su
enter [root] password
T-2 # ./one-b
Did the server start ? ( Y / n )
From another terminal use [tcp-echo-client] to test the server
T-3 $ tcp-echo-client 127.0.0.1 server's-port
Did the server run correctly? ( Y / n )
Terminate server with CTRL-C.
T-2 CTRL-C
T-2 # exit
T-2 $
End of Assignment-1.
In the previous programs the variable [ client_addr ] was used as an argument of [accept] call. [accept] call may not use this variable. In [f2.c], a short form of [ accept ] call is used. In short form of the [ accept ] call*/