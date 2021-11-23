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
pthread_t cli_writer[MAX_CONNS];
pthread_t cli_reader[MAX_CONNS];
int writernum = 0; 

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

int main() {
    
    init_data();
    
    
    // Server serv = init_server();
    // int conn2 = new_conn_listener(serv);
    
    // send_customer_info(serv.connection, customer);

    // char *buffer = (char*)&customer;
    // write(serv.connection, buffer, sizeof(customer_information_t));

    // close(serv.connection);
    // close(serv.server_socket);
    return 0;
}