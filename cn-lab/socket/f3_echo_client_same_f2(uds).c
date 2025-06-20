// file-name f3.c
// ECHO client program using stream UNIX domain socket
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#define BUFFERSIZE 1024
#define SERVER "/tmp/aa"
int main( void )
{
int sock_fd;
int n;
socklen_t length;
ssize_t i,j;
char buffer[BUFFERSIZE];
struct sockaddr_un unix_addr;
/* uncomment any one of the following three lines */
//sock_fd = socket( PF_UNIX, SOCK_STREAM, 0 );
//sock_fd = socket( PF_LOCAL, SOCK_STREAM, 0 );
sock_fd = socket( PF_FILE, SOCK_STREAM, 0 );
if( sock_fd == -1 ) { perror("socket"); exit(1); }
unix_addr.sun_family = AF_UNIX;
strncpy( unix_addr.sun_path, SERVER, sizeof(unix_addr.sun_path) );
length = SUN_LEN( &unix_addr );
n = connect( sock_fd, (struct sockaddr*) &unix_addr, length );
if( n == -1 ) { perror("connect"); exit(1); }
// clear the buffer with null characters
memset( buffer, '\0', BUFFERSIZE );
write( STDOUT_FILENO, "Enter a string ", 15 );
i = read( STDIN_FILENO, buffer, BUFFERSIZE );
// writing to server socket
j = write( sock_fd, buffer, i );
if( j == -1 ) { perror("write"); exit(1); }
// clear the buffer with null characters
memset( buffer, '\0', BUFFERSIZE );
// reading from server socket
i = read( sock_fd, buffer, BUFFERSIZE );
if( i == -1 ) { perror("read"); exit(1); }
write( STDOUT_FILENO, "Received from server->", 22 );
j = write( STDOUT_FILENO, buffer, i );
if( j == -1 ) { perror("write-2"); exit(1); }
return 0;
}
/*--------------------------------------------------------------------
Assignment-3:
(a) Compile and execute the program...
T-4 $ gcc -Wall ./f3.c -o ./uds-client
T-4 $ ./uds-client
Did the echo server-client work ? ( Y / n )
(b) Execute the client program but do not enter a string now
T-4 $ ./uds-client
In this condition client and server are connected. To verify this, run
the following command in yet another terminal.
T-5 $ netstat -axpn | grep uds
(c) Stop the server with SIGINT
T-3 CTRL-C
Run the client when server is not running
T-4 $ ./uds-client

Assignment-4:
Copy the client program under [/tmp/] directory so that other users can use the client program.
T-4 you $ cp ./uds-client /tmp/
Start the server.
T-3 you $ ./uds-server1
Login in another terminal as [ sumit ] or [ guest ] and try to run the client program.
T-6 sumit $ /tmp/uds-client
Connection would be refused as user [sumit] does not have write permission on the socket. Check with...
T-6 sumit $ ls -l /tmp/
Stop the server
T-3 CTRL-C
Edit [ f2.c ] to uncomment the three lines that change permissions.
Compile the changed program and start the server
T-3 you $ gcc -Wall ./f2.c -o ./uds-server2
T-3 you $ ./uds-server2
Run the client program again as [sumit].
T-6 sumit $ /tmp/uds-client
Did the server-client work ? ( Y / n )
End of Assignment-4.
*/