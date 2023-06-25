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
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

typedef struct User
{
    char *username;
    char *password;
} User;

typedef enum Method
{
    POST,
    UNSUPPORTED
} Method;
typedef struct Header
{
    char *name;
    char *value;
    struct Header *next;
} Header;
typedef struct StartLine
{
    enum Method method;
    char *target;
    char *version;
} StartLine;
typedef struct Request
{
    struct StartLine startline;
    struct Header *headers;
    char *body;
} Request;
//***********************************************
void setup();
Request *parse_request(const char *raw);
User *parse_body(const char *body);
//***********************************************
int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        handle_error("socket");

    // set socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        handle_error("setsockopt");

    struct sockaddr_in my_addr, client_addr;
    socklen_t client_addr_size;
    memset(&my_addr, 0, sizeof(my_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT_NUMBER);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY); // TODO: going to need a static
                                                 //  IP at some point
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1)
        handle_error("bind");

    if (listen(sockfd, BACKLOG) == -1)
        handle_error("listen");

    printf("Server listening at http://localhost:%d\n", PORT_NUMBER);
    client_addr_size = sizeof(client_addr);
    int clientfd = accept(sockfd, (struct sockaddr *)&client_addr,
                          &client_addr_size);
    if (clientfd == -1)
        handle_error("accept");

    char recv_msg[1024];
    size_t num_bytes = read(clientfd, (void *)recv_msg, sizeof(recv_msg) - 1);
    if (num_bytes == -1)
        handle_error("read");
    recv_msg[num_bytes] = '\0';
    printf("%s\n", recv_msg);
    parse_request(recv_msg);

    char response[] = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/plain\r\n"
                      "Content-Length: 13\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "\r\n"
                      "Hello, client!";
    // printf("Sending response:\t%s\n", response);
    if (write(clientfd, (void *)response, sizeof(response) - 1) == -1)
        handle_error("write");

    close(clientfd);

    close(sockfd);
    return EXIT_SUCCESS;
}
//***********************************************
// TODO: clean up memory leaks
Request *parse_request(const char *raw)
{
    Request *request = NULL;
    request = (Request *)malloc(sizeof(Request));
    if (!request)
        handle_error("malloc");

    memset(request, 0, sizeof(Request));
    char *tokenize = (char *)malloc(strlen(raw));
    strcpy(tokenize, raw);

    char *token = strtok(tokenize, " ");
    if (strcmp(token, "POST") == 0)
        request->startline.method = POST;
    else
    {
        request->startline.method = UNSUPPORTED;
        return request;
    }

    token = strtok(NULL, " ");
    request->startline.target = (char *)malloc(strlen(token));
    strcpy(request->startline.target, token);

    token = strtok(NULL, "\n");
    request->startline.version = (char *)malloc(strlen(token));
    strcpy(request->startline.version, token);

    // Headers
    request->headers = (Header *)malloc(sizeof(Header));
    token = strtok(NULL, ":");
    request->headers->name = (char *)malloc(strlen(token));
    strcpy(request->headers->name, token);

    token = strtok(NULL, "\n");
    request->headers->value = (char *)malloc(strlen(token));
    strcpy(request->headers->value, token);
    Header *travel = request->headers;
    while ((token = strtok(NULL, ":\n")))
    {
        if (strcmp(token, "\r") == 0)
        {
            // done with header section
            break;
        }
        else
        {
            travel->next = (Header *)malloc(sizeof(Header));
            travel = travel->next;

            travel->name = (char *)malloc(strlen(token));
            strcpy(travel->name, token);

            token = strtok(NULL, "\n");
            travel->value = (char *)malloc(strlen(token));
            strcpy(travel->value, token);
        }
    }

    // Body
    token = strtok(NULL, "\n");
    request->body = (char *)malloc(strlen(token));
    strcpy(request->body, token);
    parse_body(request->body);
    return request;
}
//***********************************************
User *parse_body(const char *raw)
{
    char *body = (char *)malloc(strlen(raw) + 1);
    if (body == NULL)
        handle_error("malloc");
    strcpy(body, raw);

    char *username = NULL;
    char *password = NULL;
    char *token = strtok(body, " ");
    while (token != NULL)
    {
        if (strncmp(token, "username=", strlen("username=")) == 0)
        {
            username = strdup(token + strlen("username="));
        }
        else if (strncmp(token, "password=", strlen("password=")) == 0)
        {
            password = strdup(token + strlen("password="));
        }
        token = strtok(NULL, " ");
    }

    User *usr = (User *)malloc(sizeof(User));
    usr->username = (char *)malloc(strlen(username));
    usr->password = (char *)malloc(strlen(password));
    strcpy(usr->username, username);
    strcpy(usr->password, password);
    printf("username is: %s\npassword is: %s\n", usr->username, usr->password);
    free(username);
    free(password);
    free(body);
    return usr;
}
