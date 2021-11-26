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
#include <stdint.h>

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
sem_t sem;
pthread_t tid;
pthread_t clithreads[MAX_CONNS];
int clinum = 0; 

// null vars for comparisons
customer_information_t NULL_CUSTOMER = {.id = 0, .name = {0}, .contact_number = {0}, .contact_address = {0}};
seller_information_t NULL_SELLER = {.id = 0, .name = {0}, .contact_number = {0}, .contact_address = {0}};
product_information_t NULL_PRODUCT = {.id = 0, .seller_id = 0, .quantity = 0, .price = 0, .description = {0}};
billing_information_t NULL_BILLING = {.id = 0, .customer_id = 0, .address = {0}, .price = 0};
customer_order_t NULL_ORDER = {.id = 0, .product_id = 0, .price = 0, .quantity = 0, .address = {0}};


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
            print_order(order_hash_arr[i]->order);
        }
    }
    printf("\n\n\n~~~~~~~~~~~~~~~~~ INITIALIZATION COMPLETED ~~~~~~~~~~~~~~~~~\n\n\n");
}

void recv_data(int conn) { // function for receiving data
    int flag; // init flag
    char buffer[BUFFER_SIZE]; // init buffer
    read(conn, buffer, sizeof(buffer)); // read into buffer from network socket
    flag = atoi(buffer); // set flag to first numericals from buffer
    switch(flag) { // CONTROL BLOCK
        case CUSTOMER:;
            customer_information_t cust;
            cust = recv_customer_info(conn);
            print_customer(cust);
            insert_cust(cust.id, cust);
            break;
        case SELLER:;
            seller_information_t sell;
            sell = recv_seller_info(conn);
            print_seller(sell);
            insert_sell(sell.id, sell);
            break;
        case PRODUCT:;
            product_information_t prod;
            prod = recv_product_info(conn);
            print_product(prod);
            insert_prod(prod.id, prod);
            break;
        case BILLING:;
            billing_information_t bill;
            bill = recv_billing_info(conn);
            print_billing(bill);
            insert_bill(bill.id, bill);
            break;
        case ORDER:;
            customer_order_t ord;
            ord = recv_customer_order(conn);
            print_order(ord);
            insert_order(ord.id, ord);
            break;
        default:
            error("Incorrect data type specified by Client\n");
            break;
    }
}

void* clifuncs(void* args) { // thread driver
    int conn = *((int*)args); // get connection file descriptor
    int32_t flag = 0; // define a 32-bit flag
    char buffer[BUFFER_SIZE]; // define a buffer for comms
    do {
        read(conn, buffer, sizeof(buffer)); // first handshake
        flag = atoi(buffer); // determines comm type
        bzero(buffer, sizeof(buffer)); // clear buffer for later usage
        switch(flag) { // control block for comm types
            case QUERY:;
                printf("Querying. . .\n"); 
                while(read(conn, buffer, sizeof(buffer)) == 0); // read until newline
                read(conn, buffer, sizeof(buffer)); // determine data type from network socket transmission
                int type;
                type = atoi(buffer);
                bzero(buffer, sizeof(buffer));
                // determine lookup id from network socket transmission
                while(read(conn, buffer, sizeof(buffer)) == 0);
                read(conn, buffer, sizeof(buffer));
                int id;
                id = atoi(buffer);
                sem_wait(&sem);
                switch(type) {
                    case CUSTOMER:;
                        struct cust_node *cust;
                        cust = search_cust(id);
                        if(cust == NULL) send_customer_info(conn, NULL_CUSTOMER);
                        else {
                            send_customer_info(conn, cust->customer);
                            print_customer(cust->customer);
                        }
                        break;
                    case SELLER:;
                        struct sell_node *sell;
                        sell = search_sell(id);
                        print_seller(sell->seller);
                        if(sell == NULL) send_seller_info(conn, NULL_SELLER);
                        else send_seller_info(conn, sell->seller);
                        break;
                    case PRODUCT:;
                        struct prod_node *prod;
                        prod = search_prod(id);
                        if(prod == NULL) send_product_info(conn, NULL_PRODUCT);
                        else send_product_info(conn, prod->product);
                        break;
                    case BILLING:;
                        struct bill_node *bill;
                        bill = search_bill(id);
                        if(bill == NULL) send_billing_info(conn, NULL_BILLING);
                        else send_billing_info(conn, bill->billing);
                        break;
                    case ORDER:;
                        struct order_node *ord;
                        ord = search_order(id);
                        if(ord == NULL) send_customer_order(conn, NULL_ORDER);
                        else send_customer_order(conn, ord->order);
                        break;
                    default:
                    printf("Error: Client %d sent incorrect type token (%d)\tExiting. . .\n", conn, type);
                    flag = SHUTDOWN_SIG;
                    break;
                }
                sem_post(&sem);
                break;
            case WRITE:
                sem_wait(&sem);
                recv_data(conn);
                sem_post(&sem);
                break;
            case READ:
                
                break;
            default:
                printf("Error: Client %d sent incorrect token (%d)\tExiting. . .\n", conn, flag);
                flag = SHUTDOWN_SIG;
                break;
        }
    } while(flag);
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
    sem_init(&sem, 0, 1);

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

