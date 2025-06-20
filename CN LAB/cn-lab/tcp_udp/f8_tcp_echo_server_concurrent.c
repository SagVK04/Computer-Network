//[f5.c] is a concurrent TCP ECHO server.
----------------------------------------------------------------
// file-name f5.c
// TCP ECHO concurrent server.
// usage: program-name
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<signal.h>
#include<wait.h>
#define SERVER_PORT 50000
#define BUFFERSIZE 1024
int main(int argc, char *argv[]) {
    int ser_sd;
    int tempsockfd, n;
    ssize_t i, j;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr;
    struct sigaction act;
    act.sa_handler = pipe_handler;
    sigaction(SIGPIPE, &act, NULL);
    ser_sd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ser_sd == -1) {
        perror("socket-call");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    n = bind(ser_sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(n == -1) {
        perror("bind-call");
        exit(1);
    }
    n = listen(ser_sd, 1);
    if(n == -1) {
        perror("listen-call");
        exit(1);
    }
    while(1) {
        pid_t pid, p;
        printf("%s : waiting for client's request on port %u\n", argv[0], SERVER_PORT);
        tempsockfd = accept(ser_sd, NULL, 0);
        if(tempsockfd == -1) perror("accept-call");
        pid = fork();
        if(pid == -1) {
            perror("fork-call");
            exit(1);
        }
        if(pid == 0) {
            memset(buffer, '\0', BUFFERSIZE);
            i = read(tempsockfd, buffer, BUFFERSIZE);
            if(i == -1) {
                perror("socket-read");
                exit(1);
            }
            j = write(STDOUT_FILENO,"Received from client->", 22);
            if(j == -1) perror("screen-write");
            printf("%s \n", buffer);
            j = write(tempsockfd, buffer, i);
            if(j == -1) {
                perror("socket-write");
                exit(1);
            }
            n = shutdown(tempsockfd,SHUT_RDWR);
            if(n == -1) {
                perror("shutdown");
                exit(1);
            }
            close(tempsockfd);
            exit(0);
        }
    }
}
// end of child's block
// rest is parent's block. Main closes useless socket [tempsockfd]
/*Assignment-5:
Compile and run the server
T-2 $ gcc -Wall ./f5.c -o ./five
T-2 $ ./five
In terminal-3 make connection to the server
T-3 $ tcp-echo-client 127.0.0.1 50000
Do not enter a string
In terminal-4 test the concurrent server
T-4 $ tcp-echo-client 127.0.0.1 50000
Did the server run correctly ( Y / n )
Enter a string in terminal-3
Did the server run correctly ? ( Y / n )
Check for zombie
T-3 $ ps ax
Was there zombies ? ( Y / n )
Terminate server with CTRL-C
T-2 CTRL-C
T-3 $
End of Assignment-5.
*/