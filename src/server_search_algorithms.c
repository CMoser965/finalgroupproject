/**
 * Author:  Christian Moser
 * Date:    11-21-21
 * Description: Additional read-write search functionality for server.
 * **/

// standard libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

int hashcode(int key) {
    return key % SIZE;
}

struct cust_node* cust_hash_arr[SIZE];

struct cust_node *search_cust(int key) {
    int hashed_index = hashcode(key);

    while(cust_hash_arr[hashed_index] != NULL) {
        
        if(cust_hash_arr[hashed_index]->key == key)
            return cust_hash_arr[hashed_index];
        ++hashed_index;
        hashed_index %= SIZE;
    }
    return NULL;
}

struct sell_node* sell_hash_arr[SIZE];

struct sell_node* search_sell(int key) {
    int hashed_index = hashcode(key);

    while(sell_hash_arr[hashed_index] != NULL) {
        
        if(sell_hash_arr[hashed_index]->key == key)
            return sell_hash_arr[hashed_index];
        ++hashed_index;
        hashed_index %= SIZE;
    }
    printf("No node of key %d found.\n", key);
    return NULL;
}

struct prod_node* prod_hash_arr[SIZE];

struct prod_node *search_prod(int key) {
    int hashed_index = hashcode(key);

    while(prod_hash_arr[hashed_index] != NULL) {
        
        if(prod_hash_arr[hashed_index]->key == key)
            return prod_hash_arr[hashed_index];
        ++hashed_index;
        hashed_index %= SIZE;
    }
    return NULL;
}

struct bill_node* bill_hash_arr[SIZE];

struct bill_node *search_bill(int key) {
    int hashed_index = hashcode(key);

    while(bill_hash_arr[hashed_index] != NULL) {
        
        if(bill_hash_arr[hashed_index]->key == key)
            return bill_hash_arr[hashed_index];
        ++hashed_index;
        hashed_index %= SIZE;
    }
    return NULL;
}

struct order_node* order_hash_arr[SIZE];

struct order_node *search_order(int key) {
    int hashed_index = hashcode(key);

    while(order_hash_arr[hashed_index] != NULL) {
        
        if(order_hash_arr[hashed_index]->key == key)
            return order_hash_arr[hashed_index];
        ++hashed_index;
        hashed_index %= SIZE;
    }
    return NULL;
}

void insert_cust(int key, customer_information_t temp) {
    
    struct cust_node *node = (struct cust_node*) malloc(sizeof(struct cust_node));
    node->customer = temp;
    node->key = key;

    int hashed_index = hashcode(key);

    while(cust_hash_arr[hashed_index] != NULL && cust_hash_arr[hashed_index]->key != -1) {
        ++hashed_index;
        hashed_index %= SIZE;
    }

    cust_hash_arr[hashed_index] = node;
}

void insert_sell(int key, seller_information_t temp) {

    struct sell_node *node = (struct sell_node*) malloc(sizeof(struct sell_node));
    node->seller = temp;
    node->key = key;

    int hashed_index = hashcode(key);

    while(sell_hash_arr[hashed_index] != NULL && sell_hash_arr[hashed_index]->key != -1) {
        ++hashed_index;
        hashed_index %= SIZE;
    }

    sell_hash_arr[hashed_index] = node;
}

void insert_prod(int key, product_information_t temp) {
    
    struct prod_node *node = (struct prod_node*) malloc(sizeof(struct prod_node));
    node->product = temp;
    node->key = key;

    int hashed_index = hashcode(key);

    while(prod_hash_arr[hashed_index] != NULL && prod_hash_arr[hashed_index]->key != -1) {
        ++hashed_index;
        hashed_index %= SIZE;
    }

    prod_hash_arr[hashed_index] = node;
}

void insert_bill(int key, billing_information_t temp) {
    
    struct bill_node *node = (struct bill_node*) malloc(sizeof(struct bill_node));
    node->billing = temp;
    node->key = key;

    int hashed_index = hashcode(key);

    while(bill_hash_arr[hashed_index] != NULL && bill_hash_arr[hashed_index]->key != -1) {
        ++hashed_index;
        hashed_index %= SIZE;
    }

    bill_hash_arr[hashed_index] = node;
}

void insert_order(int key, customer_order_t temp) {
    
    struct order_node *node = (struct order_node*) malloc(sizeof(struct order_node));
    node->order = temp;
    node->key = key;

    int hashed_index = hashcode(key);

    while(order_hash_arr[hashed_index] != NULL && order_hash_arr[hashed_index]->key != -1) {
        ++hashed_index;
        hashed_index %= SIZE;
    }

    order_hash_arr[hashed_index] = node;
}

void init_map(int type) {

    int n = num_entries(type);
    int i;
    switch(type) {
        case CUSTOMER: ;
            customer_information_t tempc;
            for(i = 1; i <= n; i++) {
                tempc = read_customer_info(i);
                insert_cust(tempc.id, tempc);
            }
            break;
        case SELLER: ;
            seller_information_t temps;
            for(i = 0; i < n; i++) {
                temps = read_seller_info(i);
                insert_sell(temps.id, temps);
            }
            break;
        case PRODUCT: ;
            product_information_t tempp;
            for(i = 0; i < n; i++) {
                tempp = read_product_info(i);
                insert_prod(tempp.id, tempp);
            }
            break;
        case BILLING: ;
            billing_information_t tempb;
            for(i = 0; i < n; i++) {
                tempb = read_billing_info(i);
                insert_bill(tempb.id, tempb);
            }
            break;
        case ORDER: ;
            customer_order_t tempo;
            for(i = 0; i < n; i++) {
                tempo = read_order_info(i);
                insert_order(tempo.id, tempo);
            }
            break;
    }
}