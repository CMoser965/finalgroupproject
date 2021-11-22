/**
 * Author:  Christian Moser
 * Date:    11-21-21
 * Description: Additional read-write functionality for server.
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


void write_customer_info(customer_information_t customer) {
    FILE *data;
    data = fopen("./data/customerInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(customer_information_t)];
    sprintf(buffer, "%s\t%s\t%d\t%s\n", customer.contact_address, customer.contact_number, customer.id, customer.name);
    fputs(buffer, data);
    fclose(data);
}

customer_information_t read_customer_info(int index) {
    customer_information_t customer;
    FILE *data;
    char buffer[sizeof(customer_information_t)];
    data = fopen("./data/customerInformation.txt", "r");
    fgets(buffer, sizeof(customer_information_t), (FILE*)data);
    char *short_buffer = strtok(buffer, "\n");
    int i;
    for(i = 0; i < index - 1; i++) {
        fgets(buffer, sizeof(customer_information_t), (FILE*)data);
    }
    printf("INDEX %d: %s\n", index, short_buffer);
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
    fclose(data);
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

seller_information_t read_seller_info(int index) {
    seller_information_t seller;
    FILE *data;
    char buffer[sizeof(seller_information_t)];
    data = fopen("./data/sellerInformation.txt", "r");
    fgets(buffer, sizeof(seller_information_t), (FILE*)data);
    int i = 0;
    for(int i = 0; i < index - 1; i++) {
        fgets(buffer, sizeof(seller_information_t), (FILE*)data);
    }
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

product_information_t read_product_info(int index) {
    product_information_t product;
    FILE *data;
    data = fopen("./data/productInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(product_information_t)];
    fgets(buffer, sizeof(product_information_t), (FILE*)data);
    int i;
    for(i = 0; i < index - 1; i++) {
        fgets(buffer, sizeof(product_information_t), (FILE*)data);
    }
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

// done
// writes billing on top of file
void write_billing_info(billing_information_t billing) {
    FILE *data;
    data = fopen("./data/billingInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(billing_information_t)];
    sprintf(buffer, "%d\t%d\t%s\t%d\t\n", billing.id, billing.customer_id, billing.address, billing.price);
    fputs(buffer, data);
    fclose(data);
}

// DONE
// returns the index-th billing_information from sheet
billing_information_t read_billing_info(int index) {
    billing_information_t billing;
    FILE *data;
    data = fopen("./data/billingInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(billing_information_t)];
    fgets(buffer, sizeof(billing_information_t), (FILE*)data);
    int i;
    for(i = 0; i < index - 1; i++) {
        fgets(buffer, sizeof(billing_information_t), (FILE*)data);
    }
    char *temp = strtok(buffer, "\t");
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
        temp = strtok(NULL, "\t");
    }
    return billing;
}

// DONE
// writes an order on top of entries
void write_order_info(customer_order_t order) {
    FILE *data;
    data = fopen("./data/orderInformation.txt", "a");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[sizeof(customer_order_t)];
    sprintf(buffer, "%d\t%d\t%d\t%s\t%d\t\n", order.id, order.product_id, order.quantity, order.address, order.price);
    fputs(buffer, data);
    fclose(data);
}

// DONE
// Reads index-th order entry in the orderInformation.txt
customer_order_t read_order_info(int index) {
    customer_order_t order;
    FILE *data;
    data = fopen("./data/orderInformation.txt", "r");
    if(data == NULL) error("file cannot be opened\n");
    char buffer[index*sizeof(customer_order_t)];
    fgets(buffer, index*sizeof(customer_order_t), (FILE*)data);
    int i;
    for(i = 0; i < index - 1; i++) {
        fgets(buffer, index*sizeof(customer_order_t), (FILE*)data);
    }
    char *temp = strtok(buffer, "\t");
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
        temp = strtok(NULL, "\t");
    }
    return order;
}

int num_entries(int type) {
    int num = 0;
    FILE* data;
    
    switch(type) {

        case SELLER:
            data = fopen("./data/sellerInformation.txt", "r");
            break;

        case CUSTOMER:
            data = fopen("./data/customerInformation.txt", "r");
            break;

        case PRODUCT:
            data = fopen("./data/productInformation.txt", "r");
            break;

        case BILLING: 
            data = fopen("./data/billingInformation.txt", "r");
            break;
        
        case ORDER:
            data = fopen("./data/orderInformation.txt", "r");
            break;
    }

    if(data == NULL) error("file cannot be read\n");
            char temp;
            while((temp = fgetc(data)) != EOF) {
                if(temp == '\n') ++num;
            }

    return num;
}

