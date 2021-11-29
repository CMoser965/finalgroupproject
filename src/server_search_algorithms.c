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

// // null vars for comparisons
// customer_information_t NULL_CUSTOMER = {.id = 0, .name = {0}, .contact_number = {0}, .contact_address = {0}};
// seller_information_t NULL_SELLER = {.id = 0, .name = {0}, .contact_number = {0}, .contact_address = {0}};
// product_information_t NULL_PRODUCT = {.id = 0, .seller_id = 0, .quantity = 0, .price = 0, .description = {0}};
// billing_information_t NULL_BILLING = {.id = 0, .customer_id = 0, .address = {0}, .price = 0};
// customer_order_t NULL_ORDER = {.id = 0, .product_id = 0, .price = 0, .quantity = 0, .address = {0}};

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

void overwrite_map(int type) {
    int i;
    FILE* data;
    switch(type) {
        case CUSTOMER:; 
            data = fopen("./data/customerInformation.txt", "w");
            fclose(data);
            for(i = 0; i < SIZE; i++) {
                if(cust_hash_arr[i] != NULL) 
                    write_customer_info(cust_hash_arr[i]->customer);
        }
            break;
        case SELLER:;
            data = fopen("./data/sellerInformation.txt", "w");
            fclose(data);
            for(i = 0; i < SIZE; i++) {
                if(sell_hash_arr[i] != NULL)
                    write_seller_info(sell_hash_arr[i]->seller);
            }
            break;
        case PRODUCT:;
            data = fopen("./data/productInformation.txt", "w");
            fclose(data);
            for(i = 0; i < SIZE; i++) 
                if(prod_hash_arr[i] != NULL)
                    write_product_info(prod_hash_arr[i]->product);
            break;
        case BILLING:;
            data = fopen("./data/billingInformation.txt", "w");
            fclose(data);
            for(i = 0; i < SIZE; i++) {
                if(bill_hash_arr[i] != NULL) 
                    write_billing_info(bill_hash_arr[i]->billing);
            }
            break;
        case ORDER:;
            data = fopen("./data/orderInformation.txt", "w");
            fclose(data);
            for(i = 0; i < SIZE; i++) {
                if(order_hash_arr[i] != NULL)
                    write_order_info(order_hash_arr[i]->order);
            }
            break;
        default:
            break;
    }
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
            for(i = 1; i <= n; i++) {
                temps = read_seller_info(i);
                insert_sell(temps.id, temps);
            }
            break;
        case PRODUCT: ;
            product_information_t tempp;
            for(i = 1; i <= n; i++) {
                tempp = read_product_info(i);
                insert_prod(tempp.id, tempp);
            }
            break;
        case BILLING: ;
            billing_information_t tempb;
            for(i = 1; i <= n; i++) {
                tempb = read_billing_info(i);
                insert_bill(tempb.id, tempb);
            }
            break;
        case ORDER: ;
            customer_order_t tempo;
            for(i = 1; i <= n; i++) {
                tempo = read_order_info(i);
                insert_order(tempo.id, tempo);
            }
            break;
    }
}

struct cust_node* delete_cust(struct cust_node* node) {
    int key = node->key;
    int hashed_key = hashcode(key);
    while(cust_hash_arr[hashed_key] != NULL) {
        if(cust_hash_arr[hashed_key]->key == key) {
            struct cust_node* temp = cust_hash_arr[hashed_key];
            cust_hash_arr[hashed_key] = NULL;
            return temp;
        }
        ++hashed_key;
        hashed_key %= SIZE;
    }
    return NULL;
}

struct sell_node* delete_sell(struct sell_node* node) {
    int key = node->key;
    int hashed_key = hashcode(key);
    while(sell_hash_arr[hashed_key] != NULL) {
        if(sell_hash_arr[hashed_key]->key == key) {
            struct sell_node* temp = sell_hash_arr[hashed_key];
            sell_hash_arr[hashed_key] = NULL;
            return temp;
        }
        ++hashed_key;
        hashed_key %= SIZE;
    }
    return NULL;
}
struct prod_node* delete_prod(struct prod_node* node) {
    int key = node->key;
    int hashed_key = hashcode(key);
    while(prod_hash_arr[hashed_key] != NULL) {
        if(prod_hash_arr[hashed_key]->key == key) {
            struct prod_node* temp = prod_hash_arr[hashed_key];
            prod_hash_arr[hashed_key] = NULL;
            return temp;
        }
        ++hashed_key;
        hashed_key %= SIZE;
    }
    return NULL;
}

struct bill_node* delete_bill(struct bill_node* node) {
    int key = node->key;
    int hashed_key = hashcode(key);
    while(bill_hash_arr[hashed_key] != NULL) {
        if(bill_hash_arr[hashed_key]->key == key) {
            struct bill_node* temp = bill_hash_arr[hashed_key];
            bill_hash_arr[hashed_key] = NULL;
            return temp;
        }
        ++hashed_key;
        hashed_key %= SIZE;
    }
    return NULL;
}

struct order_node* delete_order(struct order_node* node) {
    int key = node->key;
    int hashed_key = hashcode(key);
    while(order_hash_arr[hashed_key] != NULL) {
        if(order_hash_arr[hashed_key]->key == key) {
            struct order_node* temp = order_hash_arr[hashed_key];
            order_hash_arr[hashed_key] = NULL;
            return temp;
        }
        ++hashed_key;
        hashed_key %= SIZE;
    }
    return NULL;
}