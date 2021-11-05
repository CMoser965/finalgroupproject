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

void send_customer_info(int connection, customer_information_t customer) {
    if(write(connection, &customer, sizeof(customer_information_t)) < 0)
        error("Customer information could not be sent.\n");
}

customer_information_t recv_customer_info(int connection) {
    char buffer[sizeof(customer_information_t)];
    if(recv(connection, buffer, sizeof(customer_information_t), 0) < 0)
    error("Could not read customer info.\n");

    customer_information_t *temp = (customer_information_t*)&buffer;

    return *temp;
}

void send_seller_info(int connection, seller_information_t seller) {
    if(write(connection, &seller, sizeof(seller_information_t)) < 0) error("Seller information could not be sent.\n");
}

product_information_t recv_product_info(int connection) {
    char buffer[sizeof(seller_information_t)];
    if(recv(connection, buffer, sizeof(seller_information_t), 0) < 0) error("Could not read seller info.\n");

    seller_information_t *temp = (customer_information_t*)&buffer;

    return *temp;
}

void send_product_info(int connection, product_information_t seller) {
    if(write(connection, &seller, sizeof(product_information_t)) < 0) error("Seller information could not be sent.\n");
}

product_information_t recv_product_info(int connection) {
    char buffer[sizeof(product_information_t)];
    if(recv(connection, buffer, sizeof(product_information_t), 0) < 0) error("Could not read seller info.\n");

    product_information_t *temp = (product_information_t*)&buffer;

    return *temp;
}

void send_billing_info(int connection, billing_information_t seller) {
    if(write(connection, &seller, sizeof(billing_information_t)) < 0) error("Seller information could not be sent.\n");
}

billing_information_t recv_billing_info(int connection) {
    char buffer[sizeof(billing_information_t)];
    if(recv(connection, buffer, sizeof(billing_information_t), 0) < 0) error("Could not read seller info.\n");

    billing_information_t *temp = (billing_information_t*)&buffer;

    return *temp;
}

void send_customer_order(int connection, customer_order_t seller) {
    if(write(connection, &seller, sizeof(customer_order_t)) < 0) error("Seller information could not be sent.\n");
}

customer_order_t recv_customer_order(int connection) {
    char buffer[sizeof(customer_order_t)];
    if(recv(connection, buffer, sizeof(customer_order_t), 0) < 0) error("Could not read seller info.\n");

    customer_order_t *temp = (customer_order_t*)&buffer;

    return *temp;
}

int main() {
    Server serv = init_server();
    customer_information_t customer = {.contact_address = "123 Sesame Street", .contact_number = 123, .id = 1, .name = "John Smith"};
    printf("Name: %s\nID: %d\nPhone #: %d\nAddress: %s\n", customer.name, customer.id, customer.contact_number, customer.contact_address);

    send_customer_info(serv.connection, customer);

    char *buffer = (char*)&customer;
    write(serv.connection, buffer, sizeof(customer_information_t));

    close(serv.connection);
    close(serv.server_socket);
    return 0;
}