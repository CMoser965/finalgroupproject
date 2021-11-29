#define SIZE 201

struct cust_node {
    customer_information_t customer;
    int key;
};

struct sell_node {
    seller_information_t seller;
    int key;
};

struct prod_node {
    product_information_t product;
    int key;
};

struct bill_node {
    billing_information_t billing;
    int key;
};

struct order_node {
    customer_order_t order;
    int key;
};

struct cust_node *search_cust(int key);
struct sell_node *search_sell(int key);
struct prod_node *search_prod(int key);
struct bill_node *search_bill(int key);
struct order_node *search_order(int key);

void insert_cust(int key, customer_information_t temp);
void insert_sell(int key, seller_information_t temp);
void insert_prod(int key, product_information_t temp);
void insert_bill(int key, billing_information_t temp);
void insert_order(int key, customer_order_t temp);

void init_map(int type);