#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>


#define SA struct sockaddr
#define MAXBUF 512
#define PORT 50000
#define STR "This is our data"

void sendAck(int comm, int ack,int threshold);

struct head{
	int seqno,ackno;
};

struct frame{
	struct head header;
	char data[MAXBUF];
};

int main(int argc, char **argv){
	struct sockaddr_in this, sendr;
	int sock, comm; 
	socklen_t len;
	int seq, ack = 0;
	struct frame f;

	if(argc != 2){
		printf("\tUsage: %s <noise threshold level>\n\n",argv[0]);
		exit(-1);
	}

	bzero(&this, sizeof(this));
	this.sin_family = AF_INET;
	this.sin_port = htons(PORT);
	this.sin_addr.s_addr = INADDR_ANY;
	
	if((sock=socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Socket error:");
		exit(-3);
	}

	if(bind(sock, (SA*)&this, sizeof(this)) < 0){
		printf("Channel bind error\\n");
		exit(-4);
	}

	listen(sock, 10);
	len = sizeof(sendr);
	bzero(&sendr, len);
	
	if((comm = accept(sock, (SA*)&sendr, &len)) < 0){
		printf("could not establish channel\n");
		exit(-5);
	}
	printf("Channel established with %s at port %d\n",inet_ntoa(sendr.sin_addr),ntohs(sendr.sin_port));

	for(;;){
		int n = recv(comm, &f, sizeof(f), 0);
		seq = f.header.seqno;
		if(seq != ack){
			printf("Old frame received...\t");
			printf("Discarding frame.....\n");
			sendAck(comm, ack, atoi(argv[1]));
		}
		else{
			printf("Frame %d received\n", seq);
			if(seq == 0) ack = 1;
			if(seq == 1) ack = 0;
			sendAck(comm, ack, atoi(argv[1]));
		}
		if(n==0)	break;
	}
	close(comm);
	close(sock);
}

void sendAck(int comm,int ack,int threshold){
	int noise;
	struct frame fr;
	
	noise = rand()%10;
	if(noise < threshold){
		printf("Sending ACK = %d\n",ack);
		fr.header.seqno = -1;
		fr.header.ackno = ack;
		send(comm,&fr,sizeof(fr), 0);
	}else{
		printf("Channel is too noisy\t ACK is lost\n");
	}
	return;
}
