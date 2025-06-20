// file-name f1.c
// four different sockets are created by defining A or B or C or D
//stream socket in local namespace,
//datagram socket in local namespace,
//stream socket in IPv4 namespace ( TCP socket ) and
//datagram socket in IPv4 namespace ( UDP socket )



#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#define A
//#define B
//#define C
//#define D
int main (void)
{
int sd; // to be used as socket-descriptor
int n;
struct sockaddr_in d;
printf("pid = %u \n", getpid() );
#ifdef A
printf("Assignment-1a.. stream socket in local namespace\n");
sd = socket( PF_UNIX, SOCK_STREAM, 0 );
// PF_UNIX may be replaced with PF_LOCAL or PF_FILE
if( sd == -1 ) { perror("socket-call"); exit(1); }
printf("descriptor = %u \n", sd );
#endif
#ifdef B
printf("Assignment-1b... datagram socket in local namespace \n");
sd = socket( PF_UNIX, SOCK_DGRAM, 0 );
if( sd == -1 ) { perror("socket-call"); exit(1); }
printf("descriptor = %u \n", sd );
#endif
#ifdef C
printf("Assignment-1c... InternetIPv4 stream socket(TCP)\n");
sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
if( sd == -1 ) { perror("socket-call"); exit(1); }
printf("descriptor = %u \n", sd );
/* A newly created TCP socket has no remote or local address and is not fully specified. To identify the socket created in this experiment, we listen on this socket, though the socket is not yet bound to any address. This is done only to identify the type of the socket. */
n = listen( sd, 1 );
if ( n == -1 ) { perror("listen"); exit(1); }
/* When listen( ) is called on a unbound socket, the socket is automatically bound to a random free port with the local address set to all local interfaces */
#endif
#ifdef D
printf("Assignment-1d... Internet IPv4 datagram socket(UDP)\n");
sd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
if( sd == -1 ) { perror("socket-call"); exit(4); }
printf("descriptor = %u \n", sd );
/* When a UDP socket is created, its local and remote addresses are unspecified. [ listen ] call was used to identify the socket created in Assignment-1c. This call is allowed on a stream socket. As UDP is not a stream socket, method used in Assignment-1c is not used in this assignment.
To identify the created socket, the socket is bound to an Internet address 127.0.0.1 ( loopback interface ). [ bind ] system call is used to do that. An Internet address is a structure with three members. At present ignore the calls [inet_aton()], [htons()] and [bind()] calls. */
// address family
d.sin_family = AF_INET;
// port
d.sin_port = htons(0); // any port
// Internet address
if( inet_aton( "127.0.0.1" , &(d.sin_addr) ) == 0 )
{
printf("invalid-addr\n");
exit(1);
}
// the created socket is bound to address [d]
n = bind( sd, (struct sockaddr *) &d, sizeof(d) );
if( n == -1 ) { perror("bind"); exit(1); }
#endif
while ( 1 ) { sleep(1); } // endless loop
return 0;
}

/*
Assignment-1:
(a) Define A only, compile the program and run it. Ignore warning regarding unused variables.
T-3 $ gcc -Wall ./f1.c -o ./one-a
T-3 $ ./one-a
Examine the output of this command in another terminal
T-4 $ ls -l /proc/pid-value/fd
Record the following...
From output of program | From [proc] file system
| [ $ ls -l /proc/value-of-pid/fd ]
------------------------------|----------------------------------------
PID | Descriptor | Descriptor | type | Inode-number
------------------------------|-------------------------------------------
| | | |
-------------------------------------------------------------------------
Run this command.
T-4 $ netstat -xpn
OR
T-4 $ netstat -xpn | grep one-a
Record the following....
Protocol | Type | Connected-? | I-node | PID | Program-name
| STREAM/DGRAM | yes/no | | |
----------------------------------------------------------------------------------------------
| | | | |
-----------------------------------------------------------------------------------------------
Was a stream Unix socket created ? ( Y / n )
Terminate program [one-a] with CTRL-C.
(b) Define B only ( comment A, C and D ), compile and execute the program. Disregard warning about unused variables.
T-3 $ gcc -Wall ./f1.c -o ./one-b
T-3 $ ./one-b
Record the following ...
From output of program | From [proc] file system
| [ $ ls -l /proc/value-of-pid/fd ]
------------------------------|----------------------------------
PID | Descriptor | Descriptor | type | Inode-number
-------------------------------|----------------------------------------
| | | |
-----------------------------------------------------------------------
Record the following from the output of the command
T-4 $ netstat -axpn | grep one-b
Protocol | Type | Connected-? | I-node | PID | Program-name
| STREAM/DGRAM | yes/no | | |
----------------------------------------------------------------------------------------------
| | | | |
-----------------------------------------------------------------------------------------------
Was a Unix datagram socket created ? ( Y / n )
Terminate program [one-b] with CTRL-C.
(c) Define C only, ( comment A, B and D ) compile and execute the program. Disregard warning about unused variables.
T-3 $ gcc -Wall ./f1.c -o ./one-c
T-3 $ ./one-c
Record the following...
From output of program | From [proc] file system
| [ $ ls -l /proc/value-of-pid/fd ]
------------------------------|----------------------------------------
PID | Descriptor | Descriptor | type
------------------------------|-------------------------------------------
| | |
-------------------------------------------------------------------------
Run the command and save the output in file [cc]
T-4 $ netstat -atpn > ./cc
Examine the file [cc] to fill up the three blanks.
Proto | Local-Address | Foreign-Address | State | PID | Program name
| | | |
------------------------------------------------------------------------------------------
____ | *:_____ | *:* | LISTEN | ____ | /one-c
-------------------------------------------------------------------------------------------
In the above line two fields [Recv-Q] and [Send-Q] were omitted.
Was a TCP socket created ? ( Y / n )
Terminate program [one-c] with CTRL-C.
(d) Define D only, ( comment A, B and C ) compile and execute the program.
T-3 $ gcc -Wall ./f1.c -o ./one-d
T-3 $ ./one-d
Record the following...
From output of program | From [proc] file system
| [ $ ls -l /proc/value-of-pid/fd ]
------------------------------|----------------------------------------
PID | Descriptor | Descriptor | type
------------------------------|-------------------------------------------
| | |
-------------------------------------------------------------------------
In another terminal use
T-4 $ netstat -aupn
Fill up the four blanks.
Proto | Local-Address | Foreign-Address | State | PID | Program name
| | | |
------------------------------------------------------------------------------------------
____ | *:_____ | *:* | LISTEN | ____ | /one-d
-------------------------------------------------------------------------------------------
Was a UDP socket created ? ( Y / n )
Terminate program [one-d] with CTRL-C.
End of Assignment-1.
*/
