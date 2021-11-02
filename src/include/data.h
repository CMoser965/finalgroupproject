#define PORT 4000
#define BUFFER_SIZE 2048

typedef struct {
    int connection;
    int server_socket;
} Server;

void error(char *error);

Server init_server();