// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX] = {0};
    char ack0[] = "ACK0";
    char ack1[] = "ACK1";
    int expected_seq = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Receiver: Waiting for sender...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    while (1) {
        memset(buffer, 0, MAX);
        read(new_socket, buffer, MAX);

        if (strcmp(buffer, "END") == 0) {
            printf("Receiver: Transmission ended.\n");
            break;
        }

        int received_seq = buffer[0] - '0';
        printf("Receiver: Frame received with seq %d\n", received_seq);

        if (received_seq == expected_seq) {
            printf("Receiver: Frame accepted. Sending ACK%d\n", expected_seq);
            send(new_socket, expected_seq == 0 ? ack0 : ack1, 4, 0);
            expected_seq = 1 - expected_seq; // toggle expected seq
        } else {
            printf("Receiver: Duplicate frame. Resending ACK%d\n", 1 - expected_seq);
            send(new_socket, expected_seq == 1 ? ack0 : ack1, 4, 0);
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
