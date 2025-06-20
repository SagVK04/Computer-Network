#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>

//define A
//define B
//#define C
#define D


int main(void){
	int sd;
	int n;
	
	struct sockaddr_in d;

	printf("pid=%u/n",getpid());
	
	#ifdef A
	
		printf("Assignment-la.. stream socket in local namespace\n");
		sd = socket(PF_UNIX, SOCK_STREAM, 0);
		
		if(sd == -1) {
			perror("socket-call");
			exit(1);
		}

		printf("descriptor - %u\n",sd);
	#endif

	#ifdef B

		printf("Assignment-lb.. datagram socket in local namespace \n");
		sd = socket(PF_UNIX, SOCK_DGRAM, 0);
		if(sd == -1) {
			perror("socket-call");
			exit(1);
		}
		
		printf("descriptor - %u\n", sd);
	#endif

	#ifdef C
		
		 printf("Assignment-lc.. Internet IPv4 stream socket(TCP) \n");
                sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
                if(sd == -1) {
                        perror("socket-call");
                        exit(1);
                }
                

	#endif

	#ifdef D

		printf("Assignment-ld.. internet IPv4 datagram socket(UDP)\n");
		sd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sd == -1) {
                        perror("socket-call");
                        exit(4);
                }

		printf("descriptor - %u\n", sd);

		d.sin_family = AF_INET;
		d.sin_port=htons(0);
	
		if(inet_aton("127.0.0.1", &(d.sin_addr) ) == 0){
			printf("invalid-addr\n");
			exit(1);
		}

		n = bind(sd, (struct sockaddr *)&d, sizeof(d));
		if(n == -1){
			perror("bind");
			exit(1);
		}
	#endif

	while(1) {sleep(1);}

	return 0;
}

