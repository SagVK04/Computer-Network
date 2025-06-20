#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/time.h>
#include<signal.h>
#include<fcntl.h>
#define SA struct sockaddr
#define MAXBUF 512
#define port 50000
#define STR "This is our data"
#define TIMEOUT 2
struct itimerval value;
struct frame fr;
int sock;

struct head {
    int seqno, ackno;
};
struct frame {
    struct head header;
    char data[MAXBUF];
};
void handler(int sig) {
    int n, ack, seq;
    struct frame f;
    switch(sig) {
        case SIGIO:
            n = recv(sock, &f, sizeof(f), 0);
            ack = f.header.ackno;
            printf("ACK %d received\t", ack);
            fr.header.seqno = ack;
            fr.header.ackno = -1;
            strcpy(fr.data, STR);
            send(sock, &fr, sizeof(fr), 0);
            printf("Sending new frame %d\n", ack);
            value.it_value.tv_sec = TIMEOUT;
            setitimer(ITIMER_REAL, &value, 0);
            break;
        case SIGALRM:
            printf("frame %d retransmitted.....\n", fr.header.seqno);
            send(sock, &fr, sizeof(fr), 0);
            value.it_value.tv_sec = TIMEOUT;
            setitimer(ITIMER_REAL, &value, 0);
            break;
    }
}
int main(int argc, char **argv) {
    struct sockaddr_in recvr;
    int flags;
    struct sigaction ioact, alrmact;
    if(argc != 2) {
        printf("\tUsage: %s <IPaddress of the reciever machine>\n\n",argv[0]);
        exit(-1);
    }
    bzero(&recvr, sizeof(recvr));
    recvr.sin_family = AF_INET;
    recvr.sin_port = htons(PORT);
    if(inet_aton(argv[1], &recvr.sin_addr) = 0) {
        printf("Address conversion error\n");
        exit(-2);
    }
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket error: ");
        exit(-3);
    }
    if(connect(sock, (SA *)&recvr, sizeof(recvr)) < 0) {
        printf("No channel could be established\n");
        exit(-4);
    }
    printf("Channel established\n");
    ioact.sa_handler = handler;
    sigemptyset(&ioact.sa_mask);
    sigaddset(&ioact.sa_mask, SIGALRM);
    sigaction(SIGIO, &ioact, NULL);
    alrmact.sa_handler = handler;
    sigemptyset(&alrmact.sa_mask);
    sigaddset(&alrmact.sa_mask, SIGIO);
    sigaction(SIGALRM, &alrmact, NULL);
    fcntl(sock, F_SETOWN, getpid());
    fcntl(sock, F_SETFL, O_ASYNC);
    fr.header.seqno = 0;
    fr.header.ackno = -1;
    strcpy(fr.data, STR);
    printf("frame 0 transmitted\n");
    send(sock, &fr, sizeof(fr), 0);
    value.it_value.tv_sec = TIMEOUT;
    setitimer(ITIMER_REAL, &value, 0);
    for(;;) {
        sleep(10);
    }
    close(sock);
}