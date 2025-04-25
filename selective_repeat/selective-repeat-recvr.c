#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SA struct sockaddr
#define MAXBUF 50
#define PORT 50000
#define STR "This is our data"

int rlast, rnext, rwindow;
struct head {
    int seq, ack, nak;
};

struct frame {
    struct head header; char data[MAXBUF];
};

void sendAck(int comm, int ack, int threshold) {
    int noise, len; struct frame fr;
    noise = rand() % 10;
    if (noise < threshold) {
        fr.header.seq = -1; fr.header.ack = ack; fr.header.nak = -1;
        printf("Sending ACK = %d frame......\n",rnext); send(comm, &fr, sizeof(fr), 0);
    } else
        printf("Channel is too noisy\t ACK is lost\n");
    return;
}

void sendNak(int link, int nak, int threshold) {
    int noise, len;
    struct frame fr;
    noise = rand() % 10;
    if (noise < threshold) {
        fr.header.seq = -1; fr.header.ack = -1;
        fr.header.nak = nak; printf("Sending NAK %d\n",rnext);
        send(link, &fr, sizeof(fr), 0);
    }
    else
        printf("Channel is too noisy\t NAk is lost\n");
    return;
}

int main(int argc, char **argv) {
    struct sockaddr_in this, sendr;
    int sock, comm; socklen_t len;


int main(int argc, char **argv) {
    struct sockaddr_in this, sendr;
    int sock, comm; socklen_t len;
    int seqno, ackno = 0; struct frame f; int threshold;
    if (argc != 3) {
        printf("\tUsage: %s <noise threshold level> <Receive window>\n\n",argv[0]);
        exit(-1);
    }
    threshold = atoi(argv[1]);
    bzero(&this, sizeof(this));
    this.sin_family = AF_INET;
    this.sin_port = htons(PORT);
    this.sin_addr.s_addr = INADDR_ANY;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket error: ");
        exit(-3);
    }
    if (bind(sock, (SA*)&this, sizeof(this)) < 0) {
        printf("Channel bind error\n");
        exit(-4);
    }
    listen(sock, 10);
    len = sizeof(sendr);
    bzero(&sendr, len);
    if ((comm = accept(sock, (SA *)&sendr, &len)) < 0) {
        printf("could not establish channel\n");
        exit(-5);
    }
    printf("Channel established with %s at port %d\n", inet_ntoa(sendr.sin_addr),ntohs(sendr.sin_port));
    rlast = rnext = 0;
    for(;;) {
        int n = recv(comm, &f, sizeof(f), 0);
        seqno = f.header.seq;
        if ((seqno >= rnext) || (seqno <= rlast + rwindow - 1)) {
            if (seqno > rnext) {
                printf("Received out of order frame %d\t", seqno); printf("Expecting %d frame\t", rnext);
	    	sendNak(comm, rnext, threshold);
            } else {
                rnext++; printf("new ordered frame = %d received....\t",f.header.seq);
                sendAck(comm, rnext, threshold); rlast = rnext;
            }
        } else {
            printf("Old duplicate frame %d received\t", seqno);
            sendAck(comm, rnext, threshold);
        }
        if(n==0)
	    break;
    }
    close(comm);
    close(sock);
    }
}
