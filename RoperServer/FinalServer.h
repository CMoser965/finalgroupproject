/*
Created by: Roper Freeman-Vivanco

Description: This is the header file for the c program FinalServer.c
*/
#define BUFFER_SIZE 2048

#define SELLER 0
#define CUSTOMER 1
#define PRODUCT 2
#define BILLING 3
#define ORDER 4

typedef struct {
    int connection;
    int server_socket;
} Server;

typedef struct {
    char id;
    char name[BUFFER_SIZE];
    char contact_number;
    char contact_address[BUFFER_SIZE];
} SellerInfo;

typedef struct {
    int id;
    char name[BUFFER_SIZE];
    char contact_number[BUFFER_SIZE];
    char contact_address[BUFFER_SIZE];
} CustomerInfo;

typedef struct {
    int id;
    char description[BUFFER_SIZE];
    int seller_id;
    int quantity;
    int price;
} ProductInfo;

typedef struct {
    int id;
    int customer_id;
    char address[BUFFER_SIZE];
    int price;
} BillingInfo;

typedef struct {
    int id;
    int product_id;
    int quantity;
    char address[BUFFER_SIZE];
    int price;
} CustomerOrder;


// These functions are based on the send and recv functions that Christian has made so communication is clear between the servers and clients

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

void send_order_info(int connection, CustomerOrder order) {
	if (write(connection, &order, sizeof(CustomerOrder)) < 0) {
		printf("Information could not be sent.\n");
	}
}

int connect_to_data_server(int data_socket) {

	data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(data_socket < 0) {
        printf("!!!Fatal error: socket cannot be created");
        exit(1);
    }

    // specify socket address
    struct sockaddr_in data_address;
    data_address.sin_family = AF_INET;
    data_address.sin_port = htons(port);
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



void send_new_product(int connection, ProductInfo product) {
	
	if (write(connection, "21", sizeof(char[2])) < 0) {
		printf("Information could not be sent.\n");
	}
	
	send_product_info(connection, product);
}

void send_new_product(int connection, ProductInfo product) {
	
	if (write(connection, "21", sizeof(char[2])) < 0) {
		printf("Information could not be sent.\n");
	}
	
	send_product_info(connection, product);
}

void send_new_customer(int connection, CustomerInfo customer) {
	
	if (write(connection, "10", sizeof(char[2])) < 0) {
		printf("Information could not be sent.\n");
	}
	
	send_customer_info(connection, customer);
}

void send_new_seller(int connection, SellerInfo seller) {
	
	if (write(connection, "00", sizeof(char[2])) < 0) {
		printf("Information could not be sent.\n");
	}
	
	send_seller_info(connection, seller);
}

CustomerOrder recv_order_info(int connection, char[] id) {
	
	// Sending flag to the data server so I can recieve a customer order
	if (write(connection, "41", sizeof(char[2])) < 0) {
		printf("Information could not be sent.\n");
	}
	
	// Before I can recieve the order I need to send the ID
	if (write(connection, id, sizeof(id) < 0) {
		printf("Information could not be sent.\n");
	}
	
	// Now I will wait for a response
	char buffer[sizeof(CustomerOrder)];
	
	if(recv(connection, buffer, sizeof(CustomerOrder), 0) < 0) {\
		printf("Information could not be read.\n");
	}
	
	CustomerOrder *tmp = (CustomerOrder*)&buffer;
	
	// Return the customer order that was sent 
	return *tmp;
}


