#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/un.h>
#include<stdlib.h>

#define SERVER "/tmp/aa"
#define BUFFERSIZE 1024

int main(void){
	int sock_fd; int n;
	ssize_t i,j; char buffer[BUFFERSIZE];
	socklen_t length;
	struct sockaddr_un unix_addr;
	sock_fd= socket(PF_FILE, SOCK_STREAM, 0);
	if(sock_fd==-1){
		perror("socket");
		exit(1);
	}
	unix_addr.sun_family = AF_UNIX;
	strncpy(unix_addr.sun_path, SERVER, sizeof(unix_addr.sun_path));
	length = SUN_LEN( &unix_addr);
	n = connect(sock_fd, (struct sockaddr*) &unix_addr, length);
	if(n==-1){
		perror("connect");
		exit(1);
	}
	memset(buffer, '\0', BUFFERSIZE );
	write(STDOUT_FILENO, "Enter a string: ", 15);
	i = read(STDIN_FILENO, buffer, BUFFERSIZE );
	j = write(sock_fd, buffer, i);
	if(j==-1){
		perror("write");
		exit(1);
	}
	memset(buffer, '\0', BUFFERSIZE );
	i = read(sock_fd, buffer, BUFFERSIZE);
	if(i==-1){
		perror("read");
		exit(1);
	}
	write(STDOUT_FILENO, "Received from server->", 22);
	j= write(STDOUT_FILENO, buffer, i);
	if(j==-1){
	perror("write-2");
	exit(1);
	}
	return 0;
}
