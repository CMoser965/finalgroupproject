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

#include "FinalServer.h"

#define BUFFER_SIZE 2048


int port = 4001;


int main() {
	// socket id for connecting this server to the data server
	int data_socket;
	// The code below creates the connection for this server to the data server
	data_socket = connect_to_data_server(data_socket);
    
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

				// Recieve the flag from the client so the server can determine what to do next
				recv(newSock, buffer, BUFFER_SIZE, 0);
				if (strcmp(buffer, "00") == 0) {
					// This if statement case is for seller related actions
					SellerInfo seller;
					seller = recv_seller_info(newSock); // Get the seller info from client
					
					send_new_seller(data_socket, seller); // Sending the new seller info to the data server
					
					bzero(buffer, BUFFER_SIZE);
					/*
					if (recv(data_socket, buffer, BUFFER_SIZE, 0) < 0) {
						printf("Error recieving message.\n");
					} else {
						printf("Server: %s\n", buffer);
					}
					*/
						
				} else if (strcmp(buffer, "10") == 0) {
					// This if statement case is for customer related actions
					CustomerInfo customer;
					customer = recv_customer_info(newSock);
					send_new_customer(data_socket, customer);
					
				} else if (strcmp(buffer, "20") == 0) {
					// This if statement case is for product related actions
					ProductInfo product;
					product = recv_product_info(data_socket);
					send_new_product(newSock, product);
					
				} else if (strcmp(buffer, "30") == 0) {
					// This if statement case is for billing related actions
					
					
				} else if (strcmp(buffer, "41") == 0) {
					// This if statement case is for order related actions
					char id[BUFFER_SIZE];
					// This is where the server recieves the ID of the customer order they want to view
					if (recv(newSock, buffer, BUFFER_SIZE, 0) < 0) {
						printf("Error recieving message.\n");
					} 
					strcpy(id, &buffer); // copying the ID recieved from the client into the id char array
					bzero(buffer, BUFFER_SIZE);
					CustomerOrder order;
					order = recv_order_info(data_socket, id); // Getting the order info from the data server
					send_order_info(newSock, order); // sending the order info to the client 
				} 
				
					
					printf("Client: %s\n", buffer);
					send(newSock, buffer, strlen(buffer), 0);
					bzero(buffer, BUFFER_SIZE);
					
				
			}
		} 
		
	}
		
	close(newSock);
	
}