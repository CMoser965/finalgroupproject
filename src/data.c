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

/*
* 
* Function: error
* ---------------
* Prints the error code and exits with code 1
*
* error: char array describing error
*/
void error(char *error) {
    printf("Error: %s", error);
    exit(1);
}

/*
* Function: init_server
* ---------------------
* initalized a server to accept clients on PORT 
*   from IP address 127.0.0.1 (same machine)
*
* returns: struct Server with connection file descriptor and 
*   server socket file descriptor waits for connection from client
*/
Server init_server() {
    // initialize vars
    int connfd;
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

    connfd =  accept(server_socket, (struct sockaddr*)&cli_addr, &len);

    Server serv;
    serv.connection = connfd;
    serv.server_socket = server_socket;

    return serv;
}

/*
* Function: init_client
* ---------------------
* initializes client connection to specific port
* 
* port: integer value of where server port is located on network.  
* 
* returns network_socket for communication. 
*/
int init_client(int port) {
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(network_socket < 0) error("socket cannot be created.\n");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connfd = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    if(connfd < 0) error("connection cannot be made.\n");

    return network_socket;
}

int transmit(int connection, CustomerInfo information, int type) {
    return write(connection, information, sizeof(information));
}

CustomerInfo receive(int connection, int type) {
    int info_size;
    switch(type) {
        case SELLER:
            info_size = sizeof(SellerInfo);
            break;
        case CUSTOMER:
            info_size = sizeof(CustomerInfo);
            break;
        case PRODUCT:
            info_size = sizeof(ProductInfo);
            break;
        case BILLING:
            info_size = sizeof(BillingInfo);
            break;
        case ORDER:
            info_size = sizeof(CustomerOrder);
    }
    CustomerInfo buffer;
    read(connection, buffer, info_size);
    return buffer;
}

int sendCustomerInfo(int connection, char * buffer, ) {



    return 0;
}

int main() {
    Server serv = init_server();
    CustomerInfo johnsmith;
    

    transmit(serv.connection, johnsmith, CUSTOMER);

    close(serv.connection);

    return 0;
}