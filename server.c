//***********************************************
// server
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
//***********************************************
#define PORT_NUMBER 8080
#define BACKLOG 5
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
//***********************************************
void setup();
//***********************************************
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) handle_error("socket");

    // set socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
        handle_error("setsockopt");
    
    struct sockaddr_in my_addr, client_addr;
    socklen_t client_addr_size;
    memset(&my_addr, 0, sizeof(my_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT_NUMBER);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //TODO: going to need a static
                                                 // IP at some point
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        handle_error("bind");

    if (listen(sockfd, BACKLOG) == -1) handle_error("listen");

    printf("Server listening at http://localhost:%d\n", PORT_NUMBER);
    client_addr_size = sizeof(client_addr);
    int clientfd = accept(sockfd, (struct sockaddr *)&client_addr,
            &client_addr_size);
    if (clientfd == -1) handle_error("accept");

    char recv_msg[1024];
    if (read(clientfd, (void*)recv_msg, sizeof(recv_msg)) == -1)
        handle_error("read");
    printf("%s\n", recv_msg);

    char response[] = "Hello to you too";
    if (write(clientfd, (void*)response, sizeof(response)) == -1)
        handle_error("write");


    close(clientfd);

    close(sockfd);
    return EXIT_SUCCESS;
}
