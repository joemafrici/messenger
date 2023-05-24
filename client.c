//***********************************************
// client
// Author: Joe Mafrici
//***********************************************
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//***********************************************
#define DEST_IP "127.0.0.1"
#define DEST_PORT 8080
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
//***********************************************
int main()
{
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(DEST_PORT);
    server_addr.sin_addr.s_addr = inet_addr(DEST_IP);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) handle_error("socket");

    if (connect(sockfd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1) handle_error("connect");
    char msg[] = "Hello. I am the client";
    if (write(sockfd, (void*)msg, sizeof(msg)) == -1) handle_error("write");
    char recv_buff[1024];
    if (read(sockfd, (void*)recv_buff, sizeof(recv_buff)) == -1)
        handle_error("read");
    printf("%s\n", recv_buff);
    close(sockfd);
    return EXIT_SUCCESS;
}
