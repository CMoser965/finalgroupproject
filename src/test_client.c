/**
 * Author:  Christian Moser
 * Date:    11-01-21
 * Description: Test client for database server.
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

void server_query(int conn, int type, int id) {
    char buffer[BUFFER_SIZE];
    // send data type through network socket 
    sprintf(buffer, "%d", id);
    write(conn, buffer, sizeof(buffer));
    bzero(buffer, sizeof(buffer));
    switch(type) {
        case CUSTOMER:;
            customer_information_t cust;
            cust = recv_customer_info(conn);
            if(cust.id == 0) printf("Customer does not exist. \n");
            else print_customer(cust);
            break;
        case SELLER:;
            seller_information_t sell;
            sell = recv_seller_info(conn);
            if(sell.id == 0) printf("Seller does not exist. \n");
            else print_seller(sell);
            break;
        case PRODUCT:;
            product_information_t prod;
            prod = recv_product_info(conn);
            if(is_void_prod(prod)) printf("Product does not exist. \n");
            else print_product(prod);
            break;
        case BILLING:;
            billing_information_t bill;
            bill = recv_billing_info(conn);
            if(is_void_bill(bill)) printf("Billing does not exist. \n");
            else print_billing(bill);
            break;
        case ORDER:;
            customer_order_t ord;
            ord = recv_customer_order(conn);
            if(is_void_order(ord)) printf("Order does not exist. \n");
            else print_order(ord);
            break;
        default:
            error("Incorrect data type specified\n");
            break;
    }
}

void server_write(int conn, int type, char *serialized_data) {
    switch(type) {
        case CUSTOMER:;
            customer_information_t *buffer = (customer_information_t *)serialized_data;
            send_customer_info(conn, *buffer);
            break;
        case SELLER:;
            seller_information_t *sell = (seller_information_t *)serialized_data;
            send_seller_info(conn, *sell);
            break;
        case PRODUCT:;
            product_information_t *prod = (product_information_t *)serialized_data;
            send_product_info(conn, *prod);
            break;
        case BILLING:;
            billing_information_t *bill = (billing_information_t *)serialized_data;
            send_billing_info(conn, *bill);
            break;
        case ORDER:;
            customer_order_t *ord = (customer_order_t *)serialized_data;
            send_customer_order(conn, *ord);
            break;
        default:
            error("Incorrect data type specified. \n");
            break;
    }
}

int main() {
    int conn = init_client(PORT);

    customer_information_t customer = {.name = "Abe Lincoln",
                    .id=200, .contact_address="1600 Pennsylvania Ave.", .contact_number="1-800-1865"};
    seller_information_t seller = {.name = "Jeff David", .id=200,
                    .contact_number="1-800-1860", .contact_address="601 Pennsylvania Ave."};
    product_information_t product = {.description="Miracle Fish Bone Paste cures neurosis", 
                    .id=200, .seller_id=200, .price=125, .quantity=69};
    billing_information_t billing = {.price=2*product.price, .id=200, .customer_id=customer.id,
                    .address="1600 Pennsylvania Ave."};
    customer_order_t order = {.address="600 N. Wash Ln", .id=200, .price=billing.price,
                    .product_id=product.id, .quantity=product.quantity};

    int32_t flag;
    char buffer[2*BUFFER_SIZE];
    printf("Client connecting. . . \n");
    flag = 1;
    while(flag) {
        printf("Enter a comm type: \n");
        printf("1. QUERY\n");
        printf("2. WRITE\n");
        printf("3. EDIT\n");
        printf("4. DELETE\n");
        int choice;
        int temp_choice;
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                temp_choice = QUERY;
                break;
            case 2:
                temp_choice = WRITE;
                break;
            case 3: 
                temp_choice = EDIT;
                break;
            case 4:
                temp_choice = DELETE;
                break;
        }
        sprintf(buffer, "%d", temp_choice);
        write(conn, buffer, sizeof(buffer));
        printf("Sent to server-host: %s\tfrom decision %d\n", buffer, temp_choice);
        bzero(buffer, sizeof(buffer));
        switch(choice) {
            case 1:; // QUERY
                printf("Enter a data type: \n");
                printf("1. SELLER\n");
                printf("2. CUSTOMER\n");
                printf("3. PRODUCT\n");
                printf("4. BILLING\n");
                printf("5. ORDER\n");
                int new_choice = 1;
                scanf("%d", &new_choice);
                sprintf(buffer, "%d", new_choice);
                printf("Send %s\t%d\tthrough the socket\n", buffer, new_choice);
                write(conn, buffer, sizeof(buffer));
                printf("Enter ID for lookup:\n");
                int id;
                scanf("%d", &id);
                server_query(conn, new_choice, id);
                break;
            case 2:; // WRITE
                printf("Enter a data type: \n");
                printf("1. SELLER\n");
                printf("2. CUSTOMER\n");
                printf("3. PRODUCT\n");
                printf("4. BILLING\n");
                printf("5. ORDER\n");
                scanf("%d", &flag);
                printf("flag=%d\n", flag);
                sprintf(buffer, "%d", flag);
                write(conn, buffer, sizeof(buffer));
                switch(flag) {
                    case CUSTOMER:;
                        // customer_information_t *temp = &customer;
                        // server_write(conn, flag, (char*)temp);
                        // print_customer(customer);
                        send_customer_info(conn, customer);
                        break;
                    case SELLER:
                        send_seller_info(conn, seller);
                        break;
                    case PRODUCT:
                        send_product_info(conn, product);
                        break;
                    case BILLING:
                        send_billing_info(conn, billing);
                        break;
                    case ORDER:
                        send_customer_order(conn, order);
                        break;
                    default:
                        printf("Error\t\tExiting. . .\n\n");
                        flag = SHUTDOWN_SIG;
                }                
                break;
            case 3:; // EDIT
                printf("Enter a data type: \n");
                printf("1. SELLER\n");
                printf("2. CUSTOMER\n");
                printf("3. PRODUCT\n");
                printf("4. BILLING\n");
                printf("5. ORDER\n");
                new_choice = 1;
                scanf("%d", &new_choice);
                sprintf(buffer, "%d", new_choice);
                printf("Send %s\t%d\tthrough the socket\n", buffer, new_choice);
                write(conn, buffer, sizeof(buffer));
                printf("Enter ID for lookup:\n");
                id;
                scanf("%d", &id);
                server_query(conn, new_choice, id);
                switch(new_choice) {
                    case CUSTOMER:;
                        send_customer_info(conn, customer);
                        break;
                    case SELLER:;
                        send_seller_info(conn, seller);
                        break;
                    case PRODUCT:;
                        send_product_info(conn, product);
                        break;
                    case BILLING:;
                        send_billing_info(conn, billing);
                        break;
                    case ORDER:;
                        send_customer_order(conn, order);
                        break;

                }
                break;
            case 4:; // DELETE
                printf("Enter a data type: \n");
                printf("1. SELLER\n");
                printf("2. CUSTOMER\n");
                printf("3. PRODUCT\n");
                printf("4. BILLING\n");
                printf("5. ORDER\n");
                new_choice = 1;
                scanf("%d", &new_choice);
                sprintf(buffer, "%d", new_choice);
                printf("Send %s\t%d\tthrough the socket\n", buffer, new_choice);
                write(conn, buffer, sizeof(buffer));
                printf("Enter ID for lookup:\n");
                id;
                scanf("%d", &id);
                sprintf(buffer, "%d", id);
                write(conn, buffer, sizeof(buffer));
                bzero(buffer, sizeof(buffer));
                break;
        }
    }
    close(conn);
    return 0;
}