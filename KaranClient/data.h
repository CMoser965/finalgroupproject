
#define BUFFER_SIZE 10000

#define SELLER 0
#define CUSTOMER 1
#define PRODUCT 2
#define BILLING 3
#define ORDER 4

typedef struct {
    int connection;
    int server_socket;
} Server;

typedef struct{
    int id;
    char name[BUFFER_SIZE];
    char contact_number[BUFFER_SIZE];
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

typedef CustomerInfo customer_information_t;
typedef SellerInfo seller_information_t;
typedef ProductInfo product_information_t;
typedef BillingInfo billing_information_t;
typedef CustomerOrder customer_order_t;


void error(char *error);

Server init_server();

int init_client();

void send_customer_info(int connection, customer_information_t customer);
customer_information_t recv_customer_info(int connection);
void send_seller_info(int connection, seller_information_t seller);
seller_information_t recv_seller_info(int connection);
void send_product_info(int connection, product_information_t product);
product_information_t recv_product_info(int connection);
void send_billing_info(int connection, billing_information_t billing);
billing_information_t recv_billing_info(int connection);
void send_customer_order(int connection, customer_order_t order);
customer_order_t recv_customer_order(int connection);

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
/*
int connect_to_data_server(int socketfd) {

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) {
        printf("!!!Fatal error: socket cannot be created");
        exit(1);
    }

    // specify socket address
    //int port;
    //port = atoi(argv[2]);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    //server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;



    // connects to server and saves connection error message
    int connection_status;
    connection_status = connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if(connection_status == -1) {
        printf("Socket cannot connect. Exiting program.\n");
        exit(1);
    }
	
	return socketfd;
	

}
*/

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
