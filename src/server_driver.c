/**
 * Author:  Christian Moser
 * Date:    11-21-21
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
#include <semaphore.h>

// sys call libs
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

// net standard libs
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "include/data.h"
#include "include/server_read_write.h"
#include "include/server_search_algorithms.h"

#define MAX_CONNS 100

// Semaphores for multithreading
sem_t conn1, conn2;
pthread_t tid;
pthread_t clithreads[MAX_CONNS];
int clinum = 0; 

extern struct cust_node* cust_hash_arr[SIZE];
extern struct sell_node* sell_hash_arr[SIZE];
extern struct prod_node* prod_hash_arr[SIZE];
extern struct bill_node* bill_hash_arr[SIZE];
extern struct order_node* order_hash_arr[SIZE];

void init_data() {
    printf("initializing CUSTOMERS. . . \n");
    init_map(CUSTOMER);
    printf("initializing SELLERS. . . \n");
    init_map(SELLER);
    printf("initializing PRODUCTS. . .\n");
    init_map(PRODUCT);
    printf("initializing BILLING. . .\n");
    init_map(BILLING);
    printf("intiializing ORDERS. . .\n");
    init_map(ORDER);
    printf("All maps intialized!\n");

    int n = num_entries(CUSTOMER);
    int i;
    printf("~~~~~~~~~~~~~~~~~\tAll customers\t~~~~~~~~~~~~~~~~~\n\n");
    for(i = 0; i < SIZE; i++) {
        if(cust_hash_arr[i] != NULL) {
            printf("Name: %s\tID: %d\t\n", cust_hash_arr[i]->customer.name, cust_hash_arr[i]->customer.id);
        }
    }
    printf("\n~~~~~~~~~~~~~~~~~\tAll sellers\t~~~~~~~~~~~~~~~~~\n\n");
    for(i = 0; i < SIZE; i++) {
        if(sell_hash_arr[i] != NULL) {
            printf("Name: %s\tID: %d\t\n", sell_hash_arr[i]->seller.name, sell_hash_arr[i]->seller.id);
        }
    }
    printf("\n~~~~~~~~~~~~~~~~~\tAll products\t~~~~~~~~~~~~~~~~~\n\n");
    for(i = 0; i < SIZE; i++) {
        if(prod_hash_arr[i] != NULL) {
            printf("Name: %s\tID: %d\t\n", prod_hash_arr[i]->product.description, prod_hash_arr[i]->product.id);
        }
    }
    printf("\n~~~~~~~~~~~~~~~~~\tAll billing\t~~~~~~~~~~~~~~~~~\n\n");
    for(i = 0; i < SIZE; i++) {
        if(bill_hash_arr[i] != NULL) {
            printf("Order belonging to: %s\tID: %d\t\n", search_cust(bill_hash_arr[i]->billing.customer_id)->customer.name, bill_hash_arr[i]->billing.id);
        }
    }
    printf("\n~~~~~~~~~~~~~~~~~\tAll orders\t~~~~~~~~~~~~~~~~~\n\n");
    for(i = 0; i < SIZE; i++) {
        if(order_hash_arr[i] != NULL) {
            printf("Price: %d\tID: %d\t\n", order_hash_arr[i]->order.price, order_hash_arr[i]->order.id);
        }
    }
    printf("\n\n\n~~~~~~~~~~~~~~~~~ INITIALIZATION COMPLETED ~~~~~~~~~~~~~~~~~\n\n\n");
}

void* clifuncs(void* args) {
    int conn = *((int*)args);
    char buffer[BUFFER_SIZE];
    read(conn, buffer, sizeof(buffer));
    printf("Client %d: %s\n", conn, buffer);
    close(conn);
    pthread_exit(NULL);
}

int main() {
    
    init_data();
    
    // initialize vars
    int connfd;
    struct sockaddr_in serv_addr;


    // setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // set up thread locks
    socklen_t addr_size;
    sem_init(&conn1, 0, 1);
    sem_init(&conn2, 0, 1);

    // setup client connectivity
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) error("socket cannot be initialized.\n");

    // bind sock to serv addr
    int sock_bind_status = bind(server_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(sock_bind_status < 0) error("socket cannot be binded\n");

    // listen ... up to 100 servers
    int listen_status = listen(server_socket, MAX_CONNS);
    if(listen_status) error("server could not listen for client\n");

    // threading arrays for tracking threads
    pthread_t tid[MAX_CONNS];
    struct sockaddr_storage server_store;

    int count = 0;
    while(1) {
        addr_size = sizeof(server_store);
        connfd = accept(server_socket, (struct sockaddr*)&server_store, &addr_size);
        if(pthread_create(&clithreads[count++], NULL, clifuncs, &connfd)) error("failed to create thread\n");

        // CONTROL BLOCK TO JOIN THREADS AFTER MAX_CONNS CONNECT
        if(count >= MAX_CONNS) {
            count = 0;
            while(count < MAX_CONNS) {
                pthread_join(clithreads[count++], NULL);
            }
        count = 0;
        }
    }


    // close(serv.connection);
    // close(serv.server_socket);
    return 0;
}

