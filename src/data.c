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

/*
* Function: send_customer_info
* ----------------------------
* sends struct of customer information to network socket
* 
* connection: file descriptor of network socket
* customer: struct to be sent
*/
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

/*
* Function: send_seller_info
* ----------------------------
* sends struct of seller information to network socket
* 
* connection: file descriptor of network socket
* seller: struct to be sent
*/
void send_seller_info(int connection, seller_information_t seller) {
    if(write(connection, &seller, sizeof(seller_information_t)) < 0) error("Seller information could not be sent.\n");
}

seller_information_t recv_seller_info(int connection) {
    char buffer[sizeof(seller_information_t)];
    if(recv(connection, buffer, sizeof(seller_information_t), 0) < 0) error("Could not read seller info.\n");

    seller_information_t *temp = (seller_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_product_info
* ----------------------------
* sends struct of product information to network socket
* 
* connection: file descriptor of network socket
* product: struct to be sent
*/
void send_product_info(int connection, product_information_t product) {
    if(write(connection, &product, sizeof(product_information_t)) < 0) error("Seller information could not be sent.\n");
}

product_information_t recv_product_info(int connection) {
    char buffer[sizeof(product_information_t)];
    if(recv(connection, buffer, sizeof(product_information_t), 0) < 0) error("Could not read seller info.\n");

    product_information_t *temp = (product_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_billing_info
* ----------------------------
* sends struct of billing information to network socket
* 
* connection: file descriptor of network socket
* billing: struct to be sent
*/
void send_billing_info(int connection, billing_information_t billing) {
    if(write(connection, &billing, sizeof(billing_information_t)) < 0) error("Seller information could not be sent.\n");
}

billing_information_t recv_billing_info(int connection) {
    char buffer[sizeof(billing_information_t)];
    if(recv(connection, buffer, sizeof(billing_information_t), 0) < 0) error("Could not read seller info.\n");

    billing_information_t *temp = (billing_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_customer_order
* ----------------------------
* sends struct of order information to network socket
* 
* connection: file descriptor of network socket
* order: struct to be sent
*/
void send_customer_order(int connection, customer_order_t order) {
    if(write(connection, &order, sizeof(customer_order_t)) < 0) error("Seller information could not be sent.\n");
}

customer_order_t recv_customer_order(int connection) {
    char buffer[sizeof(customer_order_t)];
    if(recv(connection, buffer, sizeof(customer_order_t), 0) < 0) error("Could not read seller info.\n");

    customer_order_t *temp = (customer_order_t*)&buffer;

    return *temp;
}

void write_customer_info(customer_information_t customer) {
    FILE *data;
    data = fopen("./data/customerInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(customer_information_t)];
    sprintf(buffer, "%s\t%s\t%d\t%s\n", customer.contact_address, customer.contact_number, customer.id, customer.name);
    fputs(buffer, data);
    fclose(data);
}

customer_information_t read_customer_info() {
    customer_information_t customer;
    FILE *data;
    char buffer[sizeof(customer_information_t)];
    data = fopen("./data/customerInformation.txt", "r");
    fgets(buffer, sizeof(customer_information_t), (FILE*)data);
    char *temp = strtok(buffer, "\t");
    for(int i = 0; temp != NULL; i++) {
        switch(i) {
            case 0: // address first
                strcpy(customer.contact_address, temp);
                break;
            case 1: // phone number 
                strcpy(customer.contact_number, temp);
                break;
            case 2: // id number
                customer.id = atoi(temp);
                break;
            case 3: 
                strcpy(customer.name, temp);
        }
        temp = strtok(NULL, "\t");
    }
    return customer;
}

void write_seller_info(seller_information_t seller) {
    FILE *data;
    data = fopen("./data/sellerInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(seller_information_t)];
    sprintf(buffer, "%d\t%s\t%s\t%s\n", seller.id, seller.name, seller.contact_number, seller.contact_address);
    fputs(buffer, data);
    fclose(data);
}

seller_information_t read_seller_info() {
    seller_information_t seller;
    FILE *data;
    char buffer[sizeof(seller_information_t)];
    data = fopen("./data/sellerInformation.txt", "r");
    fgets(buffer, sizeof(seller_information_t), (FILE*)data);
    char *temp = strtok(buffer, "\t");
    for(int i = 0; temp != NULL; i++) {
        switch(i) {
            case 0:
                seller.id = atoi(temp);
                break;
            case 1:
                strcpy(seller.name, temp);
                break;
            case 2:
                strcpy(seller.contact_number, temp);
                break;
            case 3:
                strcpy(seller.contact_address, temp);
        }
        temp = strtok(NULL, "\t");
    }
    return seller;
}

void write_product_info(product_information_t product) {
    FILE *data;
    data = fopen("./data/productInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(product_information_t)];
    sprintf(buffer, "%d\t%s\t%d\t%d\t%d\n", product.id, product.description, product.seller_id, product.quantity, product.price);
    fputs(buffer, data);
    fclose(data);
}

product_information_t read_product_info() {
    product_information_t product;
    FILE *data;
    data = fopen("./data/productInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(product_information_t)];
    fgets(buffer, sizeof(product_information_t), (FILE*)data);
    char *temp = strtok(buffer, "\t");
    for(int i = 0; temp != NULL; i++) {
        switch (i) {
            case 0:
                product.id = atoi(temp);
                break;
            case 1:
                strcpy(product.description, temp);
                break;
            case 2:
                product.seller_id = atoi(temp);
                break;
            case 3:
                product.quantity = atoi(temp);
                break;
            case 4:
                product.price = atoi(temp);
        }
        temp = strtok(NULL, "\t");
    }
    return product;
    }

void write_billing_info(billing_information_t billing) {
    FILE *data;
    data = fopen("./data/billingInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(billing_information_t)];
    sprintf("%d\t%d\t%s\t%d\t\n", billing.id, billing.customer_id, billing.address, billing.price);
    fputs(buffer, data);
    fclose(data);
}

billing_information_t read_billing_info() {
    billing_information_t billing;
    FILE *data;
    data = fopen("./data/billingInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(billing_information_t)];
    fgets(buffer, sizeof(billing_information_t), (FILE*)data);
    char *temp = strtok(buffer, '\t');
    int i;
    for(i = 0; i < 4; i++) {
        switch(i) {
            case 0: // id
                billing.id = atoi(temp);
                break;
            case 1: // customer_id
                billing.customer_id = atoi(temp);
                break;
            case 2: // address
                strcpy(billing.address, temp);
                break;
            case 3: // price
                billing.price = atoi(temp);
        }
        temp = strtok(NULL, '\t');
    }
    return billing;
}

void write_order_info(customer_order_t order) {
    FILE *data;
    data = fopen("./data/orderInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(customer_order_t)];
    sprintf("%d\t%d\t%d\t%s\t%d\t\n", order.id, order.product_id, order.quantity, order.address, order.price);
    fputs(buffer, data);
    close(data);
}

customer_order_t read_order_info() {
    customer_order_t order;
    FILE *data;
    data = fopen("./data/orderInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(customer_order_t)];
    fgets(buffer, sizeof(customer_order_t), (FILE*)data);
    char *temp = strtok(buffer, '\t');
    int i;
    for(i = 0; i < 5; i++) {
        switch(i) {
            case 0: // id
                order.id = atoi(temp);
                break;
            case 1: // product_id
                order.product_id = atoi(temp);
                break;
            case 2: // quantity
                order.quantity = atoi(temp);
                break;
            case 3: // addr
                strcpy(order.address, temp);
                break;
            case 4: // price
                order.price = atoi(temp);
        }
        temp = strtok(NULL, '\t');
    }
    return order;
}

int main() {
    // Server serv = init_server();
    customer_information_t customer = {.contact_address = "123 Sesame Street", .contact_number = "1-800-420-6969", .id = 1, .name = "John Smith"};
    seller_information_t seller = {.contact_number = "1-800-666-0000", .contact_address = "601 Devel Ln.", .name = "Mr. Bidness Man", .id = 1};
    product_information_t product = {.id = 1, .description="Miracle Cure cures your skin of all herpes.", .seller_id = seller.id, .quantity = 200, .price = 100};
    billing_information_t billing = {.id = 1, .customer_id = 1, .address = customer.contact_address, billing.price = product.price};
    customer_order_t order = {.id = 1, .product_id = product.id, .quantity = 1, .address = customer.contact_address, .price = product.price};

    printf("Entered: \nName: %s\nID: %d\nPhone #: %s\nAddress: %s\n", seller.name, seller.id, seller.contact_number, seller.contact_address);
    write_seller_info(seller);
    seller_information_t new_seller = read_seller_info();
    printf("Entered: \nName: %s\nID: %d\nPhone #: %s\nAddress: %s\n", new_seller.name, new_seller.id, new_seller.contact_number, new_seller.contact_address);
    
    
    printf("Entered: \nName: %s\nID: %d\nPhone #: %s\nAddress: %s\n", customer.name, customer.id, customer.contact_number, customer.contact_address);

    write_customer_info(customer);
    customer_information_t new_customer = read_customer_info();
    printf("Retrieved: \nName: %s\nID: %d\nPhone #: %s\nAddress: %s\n", new_customer.name, new_customer.id, new_customer.contact_number, new_customer.contact_address);


    // send_customer_info(serv.connection, customer);

    // char *buffer = (char*)&customer;
    // write(serv.connection, buffer, sizeof(customer_information_t));

    // close(serv.connection);
    // close(serv.server_socket);
    return 0;
}