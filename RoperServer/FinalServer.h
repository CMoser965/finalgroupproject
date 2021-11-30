/*
Created by: Roper Freeman-Vivanco

Description: This is the header file for the c program FinalServer.c
*/
#define SELLER 1
#define CUSTOMER 2
#define PRODUCT 3
#define BILLING 4
#define ORDER 5

#define PORT 4000
#define BUFFER_SIZE 2048


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

int connect_to_data_server(int data_socket) {

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
        printf("Socket cannot connect. Exiting program.\n");
        exit(1);
    }
	
	return data_socket;
	

}




// These are the functions that Chrsitian Moser has created, I am simply utilizing them

void send_customer_info(int connection, customer_information_t customer) {
    if(write(connection, &customer, sizeof(customer_information_t)) < 0)
        error("Customer information could not be sent.\n");
}

customer_information_t recv_customer_info(int connection) {
    char buffer[sizeof(customer_information_t)];
    if(recv(connection, buffer, sizeof(customer_information_t), 0) < 0)
    error("Could not read customer info.\n");

    customer_information_t *temp = (customer_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_seller_info
* ----------------------------
* sends struct of seller information to network socket
* 
* connection: file descriptor of network socket
* seller: struct to be sent
*/
void send_seller_info(int connection, seller_information_t seller) {
    if(write(connection, &seller, sizeof(seller_information_t)) < 0) error("Seller information could not be sent.\n");
}

seller_information_t recv_seller_info(int connection) {
    char buffer[sizeof(seller_information_t)];
    if(recv(connection, buffer, sizeof(seller_information_t), 0) < 0) error("Could not read seller info.\n");

    seller_information_t *temp = (seller_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_product_info
* ----------------------------
* sends struct of product information to network socket
* 
* connection: file descriptor of network socket
* product: struct to be sent
*/
void send_product_info(int connection, product_information_t product) {
    if(write(connection, &product, sizeof(product_information_t)) < 0) error("Seller information could not be sent.\n");
}

product_information_t recv_product_info(int connection) {
    char buffer[sizeof(product_information_t)];
    if(recv(connection, buffer, sizeof(product_information_t), 0) < 0) error("Could not read seller info.\n");

    product_information_t *temp = (product_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_billing_info
* ----------------------------
* sends struct of billing information to network socket
* 
* connection: file descriptor of network socket
* billing: struct to be sent
*/
void send_billing_info(int connection, billing_information_t billing) {
    if(write(connection, &billing, sizeof(billing_information_t)) < 0) error("Seller information could not be sent.\n");
}

billing_information_t recv_billing_info(int connection) {
    char buffer[sizeof(billing_information_t)];
    if(recv(connection, buffer, sizeof(billing_information_t), 0) < 0) error("Could not read seller info.\n");

    billing_information_t *temp = (billing_information_t*)&buffer;

    return *temp;
}

/*
* Function: send_customer_order
* ----------------------------
* sends struct of order information to network socket
* 
* connection: file descriptor of network socket
* order: struct to be sent
*/
void send_customer_order(int connection, customer_order_t order) {
    if(write(connection, &order, sizeof(customer_order_t)) < 0) error("Seller information could not be sent.\n");
}

customer_order_t recv_customer_order(int connection) {
    char buffer[sizeof(customer_order_t)];
    if(recv(connection, buffer, sizeof(customer_order_t), 0) < 0) error("Could not read seller info.\n");

    customer_order_t *temp = (customer_order_t*)&buffer;

    return *temp;
}



int is_void_cust(customer_information_t temp) {
    if(temp.name == NULL && temp.id == 0 && temp.contact_address == NULL && temp.contact_number == NULL) return 1;
    else return 0;
}

int is_void_sell(seller_information_t temp) {
    if(temp.name == NULL && temp.id == 0 && temp.contact_address == NULL && temp.contact_number == NULL) return 1;
    else return 0;
}

int is_void_prod(product_information_t temp) {
    if(temp.description == NULL && temp.id == 0 && temp.quantity == 0 && temp.price == 0 && temp.seller_id == 0) return 1;
    else return 0;
}

int is_void_bill(billing_information_t temp) {
    if(temp.price == 0 && temp.id == 0 && temp.customer_id == 0 && temp.address == NULL) return 1;
    else return 0;
}

int is_void_order(customer_order_t temp) {
    if(temp.address == NULL && temp.id == 0 && temp.price == 0 && temp.product_id == 0 && temp.quantity == 0) return 1;
    else return 0;
}