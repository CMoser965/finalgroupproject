/*
Created by: Roper Freeman-Vivanco
Group: F
Email: roper.freeman@okstate.edu
Description: This is the header file for the c program FinalServer.c
*/
#define PORT 4000


typedef struct {
	int data;
	int client;
} Sockets;


int connect_to_data_server(int data_socket) {

	data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(data_socket < 0) {
        printf("!!!Fatal error: socket cannot be created");
        exit(1);
    }

    // specify socket address
    struct sockaddr_in data_address;
    data_address.sin_family = AF_INET;
    data_address.sin_port = htons(PORT);
    data_address.sin_addr.s_addr = INADDR_ANY;
    
    // connects to server and saves connection error message
    int connection_status;
    connection_status = connect(data_socket, (struct sockaddr *) &data_address, sizeof(data_address));
    if(connection_status == -1) {
        printf("Socket cannot connect. Exiting program.\n");
        exit(1);
    }
	
	
	return data_socket;
	

}

