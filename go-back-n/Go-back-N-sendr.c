#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>

#define SA struct sockaddr
#define MAXBUF 512
#define PORT 50000
#define STR "This is our data"
#define TIMEOUT 2

struct itimerval value;
struct frame fr;

int sock, window, slast, srecent;
struct head {
	int seq, ack, nak;
};
struct frame {
  struct head header;
  char data[MAXBUF];
};

void handler(int sig) {
  struct frame f;
  int ackno, seqno, nakno, n;
  switch (sig) {
    case SIGIO:
      n = recv(sock, &f, sizeof(f), 0);
      ackno = f.header.ack;
      nakno = f.header.nak;
      if ((nakno > -1) && (ackno == -1)) {
        printf("Received NAK = %d\t", nakno);
        srecent = nakno;
      } else {
        printf("ACK %d received\t", ackno);
        slast = ackno;
      }
      break;

    case SIGALRM:
      printf("SIGALRM:: Time out....going back to slast = %d\n", slast);
      srecent = slast;
      break;

    case SIGINT:
      close(sock);
      printf("\n\t.....Thanks to use this software.............\n");
      printf("\n\t Exiting..........\n");
      exit(0);
      break;
  }
}
int main(int argc, char **argv) {
    struct sockaddr_in recvr;
    struct sigaction ioact, alrmact, intact;

    if (argc != 3) {
        printf("\tUsage: %s <IPaddress of the receiver machine> <send window>\n\n", argv[0]);
        exit(-1);
    }

    window = atoi(argv[2]);
    bzero(&recvr, sizeof(recvr));
    recvr.sin_family = AF_INET;
    recvr.sin_port = htons(PORT);

    if (inet_aton(argv[1], &recvr.sin_addr) == 0) {
        printf("Address conversion error\n");
        exit(-2);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket error: ");
        exit(-3);
    }

    if (connect(sock, (SA *)&recvr, sizeof(recvr)) < 0) {
        printf("No channel could be established\n");
        exit(-4);
    }

    printf("Channel established\n");

    ioact.sa_handler = handler; sigemptyset(&ioact.sa_mask);
    sigaddset(&ioact.sa_mask, SIGALRM); ioact.sa_flags = 0;
    sigaction(SIGIO, &ioact, NULL); alrmact.sa_handler = handler; sigemptyset(&alrmact.sa_mask);
    sigaddset(&alrmact.sa_mask, SIGIO); alrmact.sa_flags = 0;
    sigaction(SIGALRM, &alrmact, NULL); intact.sa_handler = handler;
    sigemptyset(&intact.sa_mask); intact.sa_flags = 0;
    sigaction(SIGINT, &intact, NULL); fcntl(sock, F_SETOWN, getpid());
    fcntl(sock, F_SETFL, O_ASYNC); slast = srecent = 0;
    for (;;) {
        if (srecent > (slast + window - 1)) {
            printf("Window exhausted, waiting for timeout or ACK\n");
            sleep(3);
        } else {
            fr.header.seq = srecent; fr.header.ack = -1;
            fr.header.nak = -1; strcpy(fr.data, STR);
            printf("Sending frame seq no = %d\n", fr.header.seq); send(sock, &fr, sizeof(fr), 0);
            value.it_value.tv_sec = TIMEOUT; value.it_value.tv_usec = 0; 
            value.it_interval.tv_sec = 0; value.it_interval.tv_usec = 0;
            setitimer(ITIMER_REAL, &value, 0);
	    srecent++; sleep(1);
        }
    }
    close(sock); return 0;
}

