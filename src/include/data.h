#define PORT 4000
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

void write_customer_info(customer_information_t customer);
customer_information_t read_customer_info(int index);
void write_seller_info(seller_information_t seller);
seller_information_t read_seller_info(int index);
void write_product_info(product_information_t product);
product_information_t read_product_info(int index);
void write_billing_info(billing_information_t billing);
billing_information_t read_billing_info(int index);
void write_order_info(customer_order_t order);
customer_order_t read_order_info(int index);

int num_entries(int type);