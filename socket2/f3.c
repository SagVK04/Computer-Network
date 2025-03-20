#include<sys/socket.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 512
#define RECVFROM_FULL
//#define RECVFROM_STRIPPED
//#define RECV
//#define READ
//#DEFINE DEBUG
int main(int argc, char *argv[]){
	int sd,n; ssize_t i; char buffer[SIZE];
	struct sockaddr_in server_addr;
	struct sockaddr_in source_addr;
	#ifdef RECVFROM_FULL
		socklen_t source_addr_length;
		unsigned long nbo;
		unsigned long hbo;
		char *host_addr;
	#endif
	if(argc != 3){
		printf("usage -> prog-name server-address server-port \n");
		printf("example -> %s 172.16.2.15 7 \n", argv[0]);
		exit(1);
	}
	printf("udp-echo-client: PID of the program %u\n", getpid());
	sd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sd==-1){
		perror("socket-call");
		exit(1);
	}
	server_addr.sin_family = AF_INET;
	n = inet_aton(argv[1], &(server_addr.sin_addr));
	if(n==0){
		perror("invalid-address");
		exit(1);
	}
	server_addr.sin_port = htons(atoi(argv[2]));
	write(STDOUT_FILENO, "u-e-c: Enter the string:", 24);
	memset( buffer, '\0', SIZE );
	i= read(STDIN_FILENO, buffer, SIZE );
	n= sendto(sd, buffer, i, 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
	if(n==-1){
		perror("sendto-call");
		exit(1);
	}
	printf("u-e-c: Sent %u bytes to server \n", n);
	memset( buffer, '\0', SIZE );
	#ifdef RECVFROM_FULL
		printf("u-e-c: Using recvfrom() call in full form \n");
		source_addr_length = sizeof(source_addr);
		n= recvfrom( sd, buffer, SIZE, MSG_PEEK, (struct sockaddr *) &source_addr, &source_addr_length);
		if(n==-1){
			perror("recvfrom-call-full");
			exit(1);
		}
		printf("recvfrom-full: AF_INET%u \n", AF_INET);
		printf("rf-full: source address family %u\n", source_addr.sin_family);
		nbo = source_addr.sin_addr.s_addr;
		printf("rf-full: source address in network byte order ");
		printf("%lX Hex \n", nbo);
		hbo=ntohl(source_addr.sin_addr.s_addr);
		printf("rf-full: source address in host byte order = %lX Hex \n", hbo );
		host_addr=inet_ntoa(source_addr.sin_addr);
		printf("rf-full: source address in dotted decimal quad %s \n", host_addr);
		printf("rf-full: source port in network byte order = %X Hex \n", source_addr.sin_port);
		printf("rf-full: source port in host byte order = %X Hex\n", ntohs(source_addr.sin_port));
		printf("rf-full: source address length = %u bytes\n", source_addr_length);
	#endif
	#ifdef RECVFROM_STRIPPED
		printf("u-e-c: Using recvfrom() call in stripped form \n");
		n= recvfrom( sd, buffer, SIZE, 0, NULL, 0);
		if(n==-1){
			perror("recvfrom-call-stripped");
			exit(1);
		}
	#endif
	#ifdef RECV
		printf("u-e-c: Using recv() call \n");
		n=recv(sd, buffer, SIZE, MSG_PEEK);
		if(n==-1){
			perror("recvfrom-call-stripped");
			exit(1);
		}
	#endif
	#ifdef READ
		printf("u-e-c: Using read() call \n");
		n=read(sd, buffer, SIZE);
		if(n==-1){
			perror("read-call");
			exit(1);
		}
	#endif
	printf("u-e-c: Received %u bytes from server \n", n);
	write(STDOUT_FILENO, "u-e-c: From server->", 20);
	write(STDOUT_FILENO, buffer, n);
	shutdown( sd, SHUT_RDWR);
	return 0;
}
