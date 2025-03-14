#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/un.h>
#include<stdlib.h>

#define PATH "/tmp/aa"

#define BUFFERSIZE 2048

int main(void){
	int xxx; int new_sock_fd; int n;
	ssize_t i,j; char buf[BUFFERSIZE];
	socklen_t addr_len;
	struct sockaddr_un server_address;
	n=unlink(PATH);
	if(n==-1) 
		perror("unlink");
	printf("pid=%u\n",getpid());
	xxx=socket(PF_LOCAL,SOCK_STREAM,0);
	if(xxx==-1){
		perror("Socket-call");
		exit(1);
	}
	printf("Socket descriptor=%u\n",xxx);
	server_address.sun_family=AF_UNIX;
	strncpy(server_address.sun_path,PATH,sizeof(server_address.sun_path));
	addr_len=SUN_LEN(&server_address);
	n=bind(xxx,(struct sockaddr *)&server_address,addr_len);
	if(n==-1){
		perror("bind-call");
		exit(1);
	}
	n=listen(xxx,5);
	if(n==-1){
		perror("listen-call");
		exit(1);
	}
	while(1){
		printf("Waiting for a client's connection...\n");
		new_sock_fd=accept(xxx,(struct sockaddr *)&server_address,&addr_len);
		if(new_sock_fd==-1){
			perror("Accept");
			exit(1);
		}
		memset(buf,'\0',BUFFERSIZE);
		i=read(new_sock_fd,buf,BUFFERSIZE);
		if(i==-1){
			perror("read-call");
			close(new_sock_fd);
		}
		printf("uds-server: read %lu byte request from client\n",i);
		j=write(new_sock_fd,buf,i);
		if(j==-1){
			perror("write");
			exit(1);
		}
		printf("uds-server: written %lu byte reply to client\n",j);
		close(new_sock_fd);
	}
	return 0;
}
