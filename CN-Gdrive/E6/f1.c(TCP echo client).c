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

int main(int argc, char *argv[]){
	int sd;
	ssize_t i,j;
	int n;
	char buffer[BUFFERSIZE];
	struct sockaddr_in server_addr;
	
	if(argc != 3){
		printf("Usage: %s server-address server-port\n",argv[0]);
		printf("Example: %s 192.168.5.148 12345\n", argv[0]);
		exit(1);
	}

	printf("TCP-echo-client starting....\n");
	
	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sd == -1){
		perror("socket-call");
		exit(1);
	}

	/*
	struct sockaddr_in{
		sa_family_t sin_family;
		u_int16_t sin_port;
		struct in_addr sin_addr;
	};

	struct in_addr{
		u_int32_t s_addr;
	};

	*/

	server_addr.sin_family = AF_INET;

	server_addr.sin_port = htons(atoi(argv[2]));

	n = inet_aton(argv[1], &(server_addr.sin_addr));
	if(n == 0){
		printf("inet_aton-invalid address\n");
		exit(1);
	}

	n = connect(sd, (struct sockaddr*) &server_addr, sizeof(server_addr));
	if(n == -1){
		perror("Check TCP connection establishment\n");
		exit(1);
	}

	#ifdef CHECK

	printf("Check with [# netstat -tn] in another terminal\n");
	printf("Check TCP connection establishment\n");
	while(1){
		sleep(1);
	}

	#endif

	write(STDOUT_FILENO, "Enter the String:",17);
	
	memset(buffer, '\0', BUFFERSIZE);
	i=read(STDIN_FILENO, buffer, BUFFERSIZE);
	if(i == -1){
		perror("read1");
		exit(1);
	}
	printf("bytes read from keyboard = %lu\n", i);
	j = write(sd, buffer, i);
	if(j == -1){
		perror("write1");
		exit(1);
	}

	printf("bytes written in server's socket = %lu\n",j);
	
	memset(buffer, '\0', BUFFERSIZE);
	i = read(sd, buffer, BUFFERSIZE);
	if(i == -1){
		perror("read2");
		exit(1);
	}
	printf("bytes read from server's socket = %lu\n", i);
	
	write(STDOUT_FILENO, "Reply from echo server->",24);
	j = write(STDOUT_FILENO, buffer, i);
	if(j == -1){
		perror("write2");
		exit(1);
	}

	shutdown(sd, SHUT_RDWR);
	return 0;
}
