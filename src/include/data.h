#define PORT 4000
#define BUFFER_SIZE 2048

#define SELLER 1
#define CUSTOMER 2
#define PRODUCT 3
#define BILLING 4
#define ORDER 5

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

int new_conn_listener(Server serv);

void print_customer(customer_information_t temp);
void print_seller(seller_information_t temp);
void print_product(product_information_t temp);
void print_billing(billing_information_t temp);
void print_order(customer_order_t temp);

int is_void_cust(customer_information_t temp);
int is_void_sell(seller_information_t temp);
int is_void_prod(product_information_t temp);
int is_void_bill(billing_information_t temp);
int is_void_order(customer_order_t temp);