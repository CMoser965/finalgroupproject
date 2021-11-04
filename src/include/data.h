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
    int contact_number;
    char contact_address[BUFFER_SIZE];
} SellerInfo;

typedef struct {
    int id;
    char name[BUFFER_SIZE];
    int contact_number;
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

void sendCustomerInfo(int connection, CustomerInfo customer);
CustomerInfo recvCustomerInfo(int connection);
