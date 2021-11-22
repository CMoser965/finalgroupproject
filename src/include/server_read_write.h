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