// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX] = {0}, ack[MAX] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    int total_frames;
    printf("Enter number of frames to send: ");
    scanf("%d", &total_frames);

    int seq = 0;
    for (int i = 0; i < total_frames; ) {
        sprintf(buffer, "%d", seq);
        send(sock, buffer, strlen(buffer), 0);
        printf("Sender: Sent frame with seq %d. Waiting for ACK...\n", seq);

        // Timeout setup
        fd_set readfds;
        struct timeval timeout;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);

        if (activity > 0) {
            memset(ack, 0, MAX);
            read(sock, ack, MAX);
            printf("Sender: Received %s\n", ack);

            if ((seq == 0 && strcmp(ack, "ACK0") == 0) ||
                (seq == 1 && strcmp(ack, "ACK1") == 0)) {
                i++; // Next frame
                seq = 1 - seq; // Toggle sequence number
            } else {
                printf("Sender: Wrong ACK received. Resending...\n");
            }
        } else {
            printf("Sender: ACK timeout. Resending frame %d...\n", seq);
        }

        sleep(1);
    }

    // End signal
    strcpy(buffer, "END");
    send(sock, buffer, strlen(buffer), 0);
    close(sock);
    return 0;
}
