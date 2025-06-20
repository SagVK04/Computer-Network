//server code
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>

#define PATH "/tmp/aa"
#define BUFFERSIZE 2048

int main(void)
{
    int xxx;
    int new_sock_fd;
    int n;
    ssize_t i, j;
    char buf[BUFFERSIZE];
    socklen_t addr_len;
    struct sockaddr_un server_address;

    n = unlink(PATH);
    if(n == -1) perror("unlink");
    
    printf("pid = %u\n", getpid());
    
    xxx = socket(PF_LOCAL, SOCK_STREAM, 0);
    if(xxx == -1) {
        perror("socket-call");
        exit(1);
    }
    
    printf("socket descriptor = %u\n", xxx);
    
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, PATH, sizeof(server_address.sun_path));
    
    addr_len = SUN_LEN(&server_address);
    
    n = bind(xxx, (struct sockaddr *) &server_address, addr_len);
    if(n == -1) {
        perror("bind-call");
        exit(1);
    }
    
    n = listen(xxx, 5);
    if(n == -1) {
        perror("listen-call");
        exit(1);
    }
    
    while(1) {
        printf("Waiting for a client's connection\n");
        
        new_sock_fd = accept(xxx, (struct sockaddr *) &server_address, &addr_len);
        if(new_sock_fd == -1) {
            perror("accept");
            exit(1);
        }
        
        memset(buf, '0', BUFFERSIZE);
        i = read(new_sock_fd, buf, BUFFERSIZE);
        if(i == -1) {
            perror("read-call");
            close(new_sock_fd);
        }
        printf("uds-server: read %zd byte request from client\n", i);
        
        j = write(new_sock_fd, buf, i);
        if(j == -1) {
            perror("write");
            exit(1);
        }
        printf("uds-server: written %zd byte reply to client\n", j);
        
        close(new_sock_fd);
    }
    
    return 0;
}

//client 
#include <sys/types.h>  
#include <sys/un.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#define BUFFERSIZE 1024  
#define SERVER "/tmp/aa"  

int main(void)  
{  
    int sock_fd;  
    int n;  
    socklen_t length;  
    ssize_t i, j;  
    char buffer[BUFFERSIZE];  
    struct sockaddr_un unix_addr;  

    sock_fd = socket(PF_FILE, SOCK_STREAM, 0);  
    if(sock_fd == -1) { 
        perror("socket"); 
        exit(1); 
    }  

    unix_addr.sun_family = AF_UNIX;  
    strncpy(unix_addr.sun_path, SERVER, sizeof(unix_addr.sun_path));  
    length = SUN_LEN(&unix_addr);  

    n = connect(sock_fd, (struct sockaddr*) &unix_addr, length);  
    if(n == -1) { 
        perror("connect"); 
        exit(1); 
    }  

    memset(buffer, '0', BUFFERSIZE);  
    write(STDOUT_FILENO, "Enter a string ", 15);  
    i = read(STDIN_FILENO, buffer, BUFFERSIZE);  

    j = write(sock_fd, buffer, i);  
    if(j == -1) { 
        perror("write"); 
        exit(1); 
    }  

    memset(buffer, '0', BUFFERSIZE);  
    i = read(sock_fd, buffer, BUFFERSIZE);  
    if(i == -1) { 
        perror("read"); 
        exit(1); 
    }  

    write(STDOUT_FILENO, "Received from server->", 22);  
    j = write(STDOUT_FILENO, buffer, i);  
    if(j == -1) { 
        perror("write-2"); 
        exit(1); 
    }  

    return 0;  
}