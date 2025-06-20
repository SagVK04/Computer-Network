//The following program is a simple server using connection oriented protocol (stream socket), in local namespaces (Unix domain socket)
// file-name f2.c
// ECHO server using stream UNIX domain socket
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<stdlib.h>
#define PATH "/tmp/aa"
#define BUFFERSIZE 2048
int main(void) {
    int xxx;
    int new_sock_fd
    int n;
    ssize_t i,j;
    char buf[BUFFERSIZE];
    socklen_t addr_len;
    struct sockaddr_un server_address;
    n = unlink(PATH);
    if(n == -1) perror("unlink");
    printf("pid = %u\n", getpid());
    xxx = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(xxx == -1) {
        perror("socket-call");
        exit(1);
    }
    printf("socket descriptor = %u\n", xxx);
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, PATH, sizeof(server_address.sun_path));
    addr_len = SUN_LEN (&server_address);
    n = bind(xxx, (struct sockaddr *) &server_address, addr_len);
    if(n == -1) {
        perror("bind-call");
        exit(1);
    }
    n = listen(xxx, 5);
    if(n == -1) {
        perror("listen-call");
        exit(1);
    }
    while(1) {
        printf("Waiting for a client's connection\n");
        new_sock_fd = accept(xxx, (struct sockaddr *) &server_address, &addr_len);
        if(new_sock_fd == -1) {
            perror("accept");
            exit(1);
        }
        memset(buf, '\0', BUFFERSIZE);
        i = read(new_sock_fd, buf, BUFFERSIZE);
        if(i == -1) {
            perror("read-call");
            close(new_sock_fd);
        }
        printf("uds-server: read %u byte request from client\n", i);
        j = write(new_sock_fd, buf, i);
        if(j == -1) {
            perror("write");
            exit(1);
        }
        printf("uds-server: written %u byte reply to client\n", j);
        close(new_sock_fd);
    }
    return 0;
}
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