// file-name f2.c
// ECHO server using stream UNIX domain socket
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#define PATH "/tmp/aa"
#define BUFFERSIZE 2048
int main ( void )
{
int xxx; // to be used as socket descriptor
int new_sock_fd; // socket descriptor
int n;
ssize_t i,j;
char buf[BUFFERSIZE];
socklen_t addr_len;
struct sockaddr_un server_address;
// remove stale socket, if any from a previous run
n = unlink( PATH );
if( n == -1 ) perror("unlink");
printf("pid = %u\n", getpid() );
// UNIX domain socket is created.
// PF_UNIX or PF_LOCAL or PF_FILE may be used as protocol family
// xxx = socket( PF_UNIX, SOCK_STREAM, 0 );
//xxx = socket( PF_LOCAL, SOCK_STREAM, 0 );
//xxx = socket( PF_FILE, SOCK_STREAM, 0 );
xxx = socket( PF_LOCAL, SOCK_STREAM, 0 );
if( xxx == -1 ) { perror("socket-call" ); exit(1); }
printf("socket descriptor = %u\n", xxx );
// uncomment next three lines for Assignment-4
//umask(0000);
//n = fchmod( xxx,0707);
//if( n == -1 ) { perror("chmod"); }
/*
The data type for Unix domain socket is [ struct sockaddr_un ]
This has the following two members:
struct sockaddr_un {
short int sun_family // AF_UNIX
char sun_path // pathname of address
}
*/
// the above two members are initialised
server_address.sun_family = AF_UNIX;
strncpy( server_address.sun_path, PATH, sizeof(server_address.sun_path) );
/* size of the address is determined with macro [ SUN_LEN ]. Address length is needed as one of the arguments of [ bind ] system call. */
addr_len = SUN_LEN ( &server_address );
/* An address is to be assigned to the [socket] as the local endpoint of communication. This is done using the [bind] system call. Read [bind(2)]. Note that the Unix domain socket address is cast to a
generic data type [ struct sockaddr ]. */
n = bind( xxx, ( struct sockaddr *) &server_address, addr_len );
if( n == -1 ) { perror("bind-call"); exit(1); }
/* Then [ listen ] system call is used, to be in readiness to accept incoming connections to the socket */
n = listen( xxx, 5 );
if( n == -1 ) { perror("listen-call"); exit(1); }
/* The second parameter[5] indicates that five incoming requests can be kept in queue. Read manual [ listen(2) ] */
/* Then [ accept ] system call is used to accept incoming request. This system call blocks, till a client's request comes on the socket. [ accept ] system call extracts the first connection request on the queue of pending connections, creates a new connected socket and allocates a new descriptor for the socket. In the following lines this descriptor is named [ new_sock_fd ], which the [ accept ] system call returns. This new descriptor, is full duplex ( it can be read from and can be written into ) and communication is done with the client with it. Read manual [accept(2)]. */
while( 1 ) // In this endless loop a single client request is served
{
printf( "Waiting for a client's connection\n" );
new_sock_fd=accept( xxx,
(struct sockaddr *) &server_address,
&addr_len
);
if( new_sock_fd == -1 ) { perror("accept"); exit(1); }
memset( buf, '\0', BUFFERSIZE );
i = read( new_sock_fd, buf, BUFFERSIZE);
if( i == -1 ) { perror("read-call"); close(new_sock_fd ); }
printf("uds-server: read %u byte request from client\n", i );
j = write( new_sock_fd, buf, i );
if( j == -1 ) { perror("write"); exit(1); }
printf("uds-server: written %u byte reply to client\n", j );
close( new_sock_fd );
} // while ends
return 0;
} // end of main
/*----------------------------------------------------------------------
Assignment-2:
Compile and execute the program...
T-3 $ gcc -Wall ./f2.c -o ./uds-server1
T-3 $ ./uds-server1
PID =
Use the following command to find out I-Node number of the newly created socket
T-4 $ ls -l /proc/pid-of-process/fd
I-Node number =
Check creation of file( socket )
T-4 $ ls -l /tmp | grep aa
Check type of the file
T-4 $ file /tmp/aa
Use the following command.
T-4 $ netstat -xapn
Examine the output and fill up the following table
Protocol Type State I-Node PID Path
Keep this server running in this terminal. A client program would connect to this server.
End of Assignment-2.
*/