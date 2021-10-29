/**
 * Author:  Christian Moser
 * Date:    10-28-21
 * Description: Driver file for data server.
 * **/

// standard libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// threading libs
#include <pthread.h>
#include <signal.h>

// sys call libs
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

// net standard libs
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "include/data.h"

void error(char *error) {
    printf("Error: %s", error);
    exit(1);
}


int init_server() {
    // initialize vars
    int connection fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t len = sizeof(cli_addr);

    // setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // setup client connectivity
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) error("socket cannot be initialized.\n");

    // bind sock to serv addr
    int sock_bind_status = bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(sock_bind_status < 0) error("socket cannot be binded\n");

    // listen ... up to 100 servers
    int listen_status = listen(server_socket, 100);
    if(listen_status) error("server could not listen for client\n");

    return accept(server_socket, (struct sockaddr*)&cli_addr, &len);

}