#define _GNU_SOURCE
#define MAXLINE 1024

#define PORT 4000
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

#include "data.h"

#define BUFFER_SIZE 2048



void send_customer_info(int connection, CustomerInfo customer) {
	if (write(connection, &customer, sizeof(CustomerInfo)) < 0) {
		printf("Information could not be sent.\n");
	}
}

CustomerInfo recv_customer_info(int connection) {
	char buffer[sizeof(CustomerInfo)];
	
	if(recv(connection, buffer, sizeof(CustomerInfo), 0) < 0) {\
		printf("Information could not be read.\n");
	}
	
	CustomerInfo *tmp = (CustomerInfo*)&buffer;
	
	return *tmp;
	
}

void send_seller_info(int connection, SellerInfo seller) {
	if (write(connection, &seller, sizeof(SellerInfo)) < 0) {
		printf("Information could not be sent.\n");
	}
}

SellerInfo recv_seller_info(int connection) {
	char buffer[sizeof(SellerInfo)];
	
	if(recv(connection, buffer, sizeof(SellerInfo), 0) < 0) {\
		printf("Information could not be read.\n");
	}
	
	SellerInfo *tmp = (SellerInfo*)&buffer;
	
	return *tmp;
}

void send_product_info(int connection, ProductInfo product) {
	if (write(connection, &product, sizeof(ProductInfo)) < 0) {
		printf("Information could not be sent.\n");
	}
}

ProductInfo recv_product_info(int connection) {
	char buffer[sizeof(ProductInfo)];
	
	if(recv(connection, buffer, sizeof(ProductInfo), 0) < 0) {\
		printf("Information could not be read.\n");
	}
	
	ProductInfo *tmp = (ProductInfo*)&buffer;
	
	return *tmp;
}


int main() {
	// socket id for connecting this server to the data server
	int data_socket;
	// The code below creates the connection for this server to the data server
	
    data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(data_socket < 0) {
        printf("!!!Fatal error: socket cannot be created");
        exit(1);
    }

    // specify socket address
    struct sockaddr_in data_address;
    data_address.sin_family = AF_INET;
    data_address.sin_port = htons(4001);
    data_address.sin_addr.s_addr = INADDR_ANY;
    
    // connects to server and saves connection error message
    int connection_status;
    connection_status = connect(data_socket, (struct sockaddr *) &data_address, sizeof(data_address));
    if(connection_status == -1) {
        printf("!!!Fatal Error: Socket cannot connect\n");
        exit(1);
    }
	
	
	
	
	// The code below is for establishing a connection between the multiple clients and this server
	int socketfd, retval;  // used for the accept() function
	int newSock;
	
	pid_t childpid; // used for subproccesses
	
	socklen_t addrSize;
	
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
	
	serverAd.sin_port = htons(PORT);
	
	serverAd.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	// binding the socket to the address
	retval = bind(socketfd, (struct sockaddr*)&serverAd, sizeof(serverAd));
	
	if ( retval < 0) {
		error("Error: Socket could not be binded.\n");
	} else {
		printf("Socket binding was successful.\n");
	}
	
	// Listening for connections
	
	if (listen(socketfd, MAX_CLI) == 0) {
		printf("Server is now listening for connections.\n");
	} else {
		error("Error: Server was not able to listen to client.\n");
	}
	
	
	
	// indefinite loop for listening
	
	while(1) {
		
		newSock = accept(socketfd, (struct sockaddr*)&clientAd, &addrSize);

		if (newSock < 0) {
			error("Error: The client could not be accepted.\n");
		} 
		printf("Connection accepted from %s:%d\n", inet_ntoa(clientAd.sin_addr), ntohs(clientAd.sin_port));
		
		if ((childpid = fork()) == 0) { // This is the child case
			close(socketfd);

			while(1) {

				recv(newSock, buffer, BUFFER_SIZE, 0);
				
				if (strcmp(buffer, "0") == 0) {
					// This if statement case is for seller related actions
					SellerInfo seller;
					seller = recv_seller_info(newSock);
					send_seller_info(data_socket, seller);
					
				} else if (strcmp(buffer, "1") == 0) {
					// This if statement case is for customer related actions
					CustomerInfo customer;
					customer = recv_customer_info(newSock);
					send_customer_info(data_socket, customer);
					
				} else if (strcmp(buffer, "2") == 0) {
					// This if statement case is for product related actions
					ProductInfo product;
					product = recv_product_info(data_socket);
					send_product_info(newSock, product);
					
				} else if (strcmp(buffer, "3") == 0) {
					// This if statement case is for billing related actions
					
					
				} else if (strcmp(buffer, "4") == 0) {
					// This if statement case is for order related actions
					
					
				} 
				
					
					printf("Client: %s\n", buffer);
					send(newSock, buffer, strlen(buffer), 0);
					bzero(buffer, BUFFER_SIZE);
					
				
			}
		} 
		
	}
		
	close(newSock);
	
}
