#define PORT 4000
#define BUFFER_SIZE 2048

typedef struct {
    int connection;
    int server_socket;
} Server;

typedef struct {
    int id;
    char *name;
    int contact_number;
    char *contact_address;
} SellerInfo;

typedef struct {
    int id
    char *name;
    int contact_number;
    char *contact_address;
} CustomerInfo;

typedef struct {
    int id;
    char *description;
    int seller_id;
    int quantity;
    int price;
} ProductInfo;

typedef struct {
    int id;
    int customer_id;
    char *address;
    int price;
} BillingInfo;

typedef struct {
    int id;
    int product_id;
    int quantity;
    char *address;
    int price;
} CustomerOrder;



void error(char *error);

Server init_server();

int init_client();