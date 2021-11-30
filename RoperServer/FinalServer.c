/*
Created by: Roper Freeman-Vivanco
Description: This is the file for the server that is the intermediate program that communicates
					information between the data server and the client.
*/


#define _GNU_SOURCE
#define MAXLINE 1024

#define SA struct sockaddr
#define MAX_CLI 100
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>

#include "FinalServer.h"


#define MAX_CONNS 100
#define SHUTDOWN_SIG 0
#define QUERY 10
#define	WRITE 11
#define	EDIT 12
#define	DELETE 13

int port = 4001;


sem_t sem;
pthread_t tid;
pthread_t clithreads[MAX_CONNS];
int clinum = 0;



void get_query(int data, int client) {
	char buffer[3*BUFFER_SIZE];
	
	
	sprintf(buffer, "%d", QUERY); 
	write(data, buffer, sizeof(buffer)); // writing to the data server about the type of info transfer
	bzero(buffer, sizeof(buffer));
	
	read(client, buffer, sizeof(buffer));
	int dataType;
	dataType = atoi(buffer); // getting the type of data that they want to query
	bzero(buffer, sizeof(buffer)); // resetting the buffer
	
	
	sprintf(buffer, "%d", dataType); 
	write(data, buffer, sizeof(buffer));  // writing to the server about the data type
	bzero(buffer, sizeof(buffer));
	
	
	read(client, buffer, sizeof(buffer));
	int id;
	id = atoi(buffer); // getting the id of the data that they want to find
	bzero(buffer, sizeof(buffer)); // resetting the buffer
	
	sprintf(buffer, "%d", id); 
	write(data, buffer, sizeof(buffer));  // writing the id to the server for query
	bzero(buffer, sizeof(buffer));
	
	switch(dataType) {
		case CUSTOMER: ;
			// customer query case

			customer_information_t customer;
			customer = recv_customer_info(data);
			if (customer.id == 0) {
				char message[] = "Customer ID does not exist.\n";
				write(client, message, sizeof(message));
			} else {
				send_customer_info(client, customer); 
			}
			bzero(buffer, sizeof(buffer));
			break;
			
			case SELLER: ;
				// seller query case
				seller_information_t seller;
				seller = recv_seller_info(data);
				if (seller.id == 0) {
					char message[] = "Seller ID does not exist.\n";
					write(client, message, sizeof(message));
				} else {
					send_seller_info(client, seller); 
				}
				bzero(buffer, sizeof(buffer));
				break;
				
			case PRODUCT: ;
				// product query case
				int cont = 1;
				
				while(cont) {
					product_information_t product;
					product = recv_product_info(data);
					
					if (is_void_prod(product)) {
						send_product_info(client, product);
						cont = 0;
					} else {
						send_product_info(client, product); 
					}
					bzero(buffer, sizeof(buffer));
				}
				
				break;
			
			case BILLING: ;
				// billing query case
				
				billing_information_t bill;
				bill = recv_billing_info(data);
				if (is_void_bill(bill)) {
					char message[] = "Billing does not exist.\n";
					write(client, message, sizeof(message));
				} else {
					send_billing_info(client, bill); 
				}
				bzero(buffer, sizeof(buffer));
				break;
			
			case ORDER: ;
				// order query case
				
				customer_order_t order;
				order = recv_customer_order(data);
				if (is_void_order(order)) {
					char message[] = "Order does not exist.\n";
					write(client, message, sizeof(message));
				} else {
					send_customer_order(client, order); 
				}
				bzero(buffer, sizeof(buffer));
				break;
			
			default:
				error("Incorrect data type specified\n");
				break;
	}
}

void get_write(int data, int client) {
	char buffer[3*BUFFER_SIZE];
	
	sprintf(buffer, "%d", WRITE); 
	write(data, buffer, sizeof(buffer)); // writing to the data server about the type of info transfer
	bzero(buffer, sizeof(buffer));
	
	read(client, buffer, sizeof(buffer));
	int dataType;
	dataType = atoi(buffer); // getting the type of data that they want to write
	bzero(buffer, sizeof(buffer)); // resetting the buffer
	
	switch(dataType) {
		case CUSTOMER: ;
			// customer write case

			customer_information_t customer;
			customer = recv_customer_info(client); // get the customer from the client
			send_customer_info(data, customer); // send the new customer over to the data server
			bzero(buffer, sizeof(buffer)); // resetting the buffer
			
			break;
			
			case SELLER: ;
				// seller write case
				seller_information_t seller;
				seller = recv_seller_info(client);
				send_seller_info(data, seller);
				bzero(buffer, sizeof(buffer));
				break;
				
			case PRODUCT: ;
				// product write case
				
				read(client, buffer, sizeof(buffer));
				int id;
				id = atoi(buffer); // getting the id of the seller who wants to list the product 
				bzero(buffer, sizeof(buffer)); // resetting the buffer
				
				sprintf(buffer, "%d", id); 
				write(data, buffer, sizeof(buffer)); // writing the id of the seller to the data server
				bzero(buffer, sizeof(buffer));
				
				product_information_t product;
				product = recv_product_info(client);
				send_product_info(data, product);
				bzero(buffer, sizeof(buffer));
				break;
				
			default:
				error("Incorrect data type specified\n");
				break;
	}
}

void get_edit(int data, int client) {
	char buffer[3*BUFFER_SIZE];
	
	
	sprintf(buffer, "%d", EDIT); 
	write(data, buffer, sizeof(buffer)); // writing to the data server about the type of info transfer
	bzero(buffer, sizeof(buffer));
	
	read(client, buffer, sizeof(buffer));
	int dataType;
	dataType = atoi(buffer); // getting the type of data that they want to write
	bzero(buffer, sizeof(buffer)); // resetting the buffer
	
	switch(dataType) {
		case CUSTOMER: ;
			// customer edit case

			customer_information_t customer;
			customer = recv_customer_info(client); // get the customer from the client
			send_customer_info(data, customer); // send the new customer over to the data server
			bzero(buffer, sizeof(buffer)); // resetting the buffer
			
			break;
			
			case SELLER: ;
				// seller edit case
				seller_information_t seller;
				seller = recv_seller_info(client);
				send_seller_info(data, seller);
				bzero(buffer, sizeof(buffer));
				break;
				
			case PRODUCT: ;
				// product edit case
				
				read(client, buffer, sizeof(buffer));
				int id;
				id = atoi(buffer); // getting the id of the seller who wants to list the product 
				bzero(buffer, sizeof(buffer)); // resetting the buffer
				
				sprintf(buffer, "%d", id); 
				write(data, buffer, sizeof(buffer)); // writing the id of the seller to the data server
				bzero(buffer, sizeof(buffer));
				
				
				product_information_t product;
				product = recv_product_info(client);
				send_product_info(data, product);
				bzero(buffer, sizeof(buffer));
				break;
				
			default:
				error("Incorrect data type specified\n");
				break;
	}
}

void get_delete(int data, int client) {
	char buffer[3*BUFFER_SIZE];
	
	
	sprintf(buffer, "%d", DELETE); 
	write(data, buffer, sizeof(buffer)); // writing to the data server about the type of info transfer
	bzero(buffer, sizeof(buffer));
	
	read(client, buffer, sizeof(buffer));
	int dataType;
	dataType = atoi(buffer); // getting the type of data that they want to write
	bzero(buffer, sizeof(buffer)); // resetting the buffer
	
	
	sprintf(buffer, "%d", dataType); 
	write(data, buffer, sizeof(buffer)); // writing to the data server about the type of info to delete
	bzero(buffer, sizeof(buffer));
	
	
	read(client, buffer, sizeof(buffer));
	int id;
	id = atoi(buffer); // getting the id of the data that they want to find
	bzero(buffer, sizeof(buffer)); // resetting the buffer
	
	sprintf(buffer, "%d", id); 
	write(data, buffer, sizeof(buffer));  // writing the id to the server for query
	bzero(buffer, sizeof(buffer));
	
	
	
}







void* clientFunc(void* args)  {
	
	unsigned int (*conn)[2] = (unsigned int(*)[2])args;
	int data = conn[0];
	int client = conn[1];
	clinum++;
	
	int32_t flag = 0;
	int connected = 1;
	char buffer[BUFFER_SIZE];
	
	do {
		read(client, buffer, sizeof(buffer));
		flag = atoi(buffer);
		while(read(client, buffer, sizeof(buffer)) == 0);
		bzero(buffer, sizeof(buffer));
		
		switch(flag) {
			case QUERY: ;
			
				sem_wait(&sem);
				get_query(data, client);
				sem_post(&sem);
				bzero(buffer, sizeof(buffer));
			
			case WRITE: ;
			
				sem_wait(&sem);
				get_write(data, client);
				sem_post(&sem);
				bzero(buffer, sizeof(buffer));
			
			case EDIT: ;
			
				sem_wait(&sem);
				get_edit(data, client);
				sem_post(&sem);
				bzero(buffer, sizeof(buffer));
				
			case DELETE: ;
			
				sem_wait(&sem);
				get_delete(data, client);
				sem_post(&sem);
				bzero(buffer, sizeof(buffer));
				
			default:
				break;
		}
	} while(connected);
	
	clinum--;
	pthread_exit(NULL);
	
}






int main() {
	// socket id for connecting this server to the data server
	int data_socket;
	// The code below creates the connection for this server to the data server
	data_socket = connect_to_data_server(data_socket);
    
	// The code below is for establishing a connection between the multiple clients and this server
	int socketfd, retval;  // used for the accept() function
	int newSock;
	
	
	socklen_t addrSize;
	sem_init(&sem, 0, 1);
	
	
	struct sockaddr_in serverAd, clientAd;  // initializing the server and client address holder
	char buffer[BUFFER_SIZE];
	
	
	socketfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketfd < 0) {
		error("Error: socket cannot be defined\n");
	} else {
		printf("Socket created successfully.\n");
	}
		
	memset(&serverAd, '\0', sizeof(serverAd));
	serverAd.sin_family = AF_INET;
	serverAd.sin_addr.s_addr = inet_addr("127.0.0.1");
		
	while(1) {
		// binding the socket to the address and changing the port, if it is taken, until a valid port is found and binded
		serverAd.sin_port = htons(port);
		retval = bind(socketfd, (struct sockaddr*)&serverAd, sizeof(serverAd));
		
		if ( retval < 0) {
			printf("Error: Socket could not be binded to port %d. Trying another port.\n", port);
			port = port + 1;
		} else {
			printf("Socket binding was successful.\n");
			break;
		}
	}
	
	// Listening for connections
	if (listen(socketfd, MAX_CLI) == 0) {
		printf("Server is now listening for connections on port %d.\n", port);
	} else {
		error("Error: Server was not able to listen to client.\n");
	}
	
	
	
	pthread_t tid[MAX_CONNS];
	struct sockaddr_storage server_store;
	
	int count = 0;
	int socketArray[2];
	socketArray[0] = data_socket;
	// indefinite loop for listening
	while(1) {
		addrSize = sizeof(server_store);
		newSock = accept(socketfd, (struct sockaddr*)&clientAd, &addrSize);
		
		socketArray[1] = newSock;
		
		
		if (pthread_create(&clithreads[count++], NULL, clientFunc, &socketArray)) error("Failed to create the thread.\n");
		
		if (count >= MAX_CONNS) {
			count = 0;
			while(count < MAX_CONNS) {
				pthread_join(clithreads[count++], NULL);
			}
			count = 0;
		}
	}
		
		
		return 0;
		
}