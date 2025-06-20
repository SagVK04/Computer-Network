//The program [f1.c] is the TCP [echo] client. This program has to call [socket] and [connect] calls to establish connection with a server. Then the client sends request to server and reads reply from server.
//file-name f1.c TCP echo client
// usage -> program-name server-address server-port
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define BUFFERSIZE 1024
//#define CHECK
int main(int argc, char *argv[]) {
    int sd;
    ssize_t i,j;
    int n;
    char buffer[BUFFERSIZE];
    struct sockaddr_in server_addr;
    if(argc != 3) {
        printf("Usage: %s server-address server-port \n", argv[0]);
        printf("Example: %s 192.168.5.148 12345 \n", argv[0]);
        exit(1);
    }
    printf("TCP-echo-client starting...\n");
    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sd == -1) {
        perror("socket-call");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    n = inet_aton(argv[1], &(server_addr.sin_addr));
    if(n == 0) {
        printf("inet_aton-Invalid address\n");
        exit(1);
    }
    n = connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(n == -1) {
        perror("connect-call");
        exit(1);
    }
    #ifdef CHECK
    printf("Check with [ # netstat -tn ] in another terminal\n");
    printf("Check TCP connection establishment\n");
    while(1) {
        sleep(1);
    }
    #endif
    write(STDOUT_FILENO, "Enter the string: ", 17);
    memset(buffer, '\0', BUFFERSIZE);
    i = read(STDIN_FILENO, buffer, BUFFERSIZE);
    if(i == -1) {
        perror("read1");
        exit(1);
    }
    printf("bytes read from keyboard = %u\n", i);
    j = write(sd, buffer, i);
    if(j == -1) {
        perror("write1");
        exit(1);
    }
    printf("bytes written in server's socket = %u\n",j);
    memset(buffer, '\0', BUFFERSIZE);
    i = read(sd, buffer, BUFFERSIZE);
    if(i == -1) {
        perror("read2");
        exit(1);
    }
    printf("bytes read from server's socket = %u\n", i);
    write(STDOUT_FILENO,"Reply from echo server->", 24);
    j = write(STDOUT_FILENO, buffer, i);
    if(j == -1) {
        perror("write2");
        exit(1);
    }
    shutdown(sd, SHUT_RDWR);
    return 0;
}

/*$ gcc -Wall ./f1.c -o ./one-a
Run the program using wrong number of arguments to see usage message
$ ./one-a
$ ./one-a 192.168.5.251
Run the program correctly. Replace 192.168.5.251 with a suitable server address
$ ./one-a 192.168.5.251 7*/