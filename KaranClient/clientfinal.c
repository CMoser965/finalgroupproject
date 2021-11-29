/* ************************************************************************************
 Author Name: Karan Bhakta                           Student ID: A20057064
 Email: kbhakta@okstate.edu
 Date: 11/2/2021
 Program Description: Main file for the Client Communication
* ********************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <math.h>
#include <time.h>

#include "data.h"
int id();

/*
//void productInfo();
//void billinginfo();
//void customerOrder();

// initialize a struct, and store the info in to the struct


// log the customers 
void productInfo(int connection, product_information_t product)
{
    
	printf("Product ID: ", id()); // create a function to randomly generate a ID (use the one from the precious assignment) 
	printf("Product Description: ");
	scanf("%c\n", product.description); 
	printf("Seller ID: ");
	scanf("%d\n", &product.seller_id);
    printf("Product Quantity: ");
	scanf("%d\n", &product.quantity);
    printf("Product Price: ");
	scanf("%d\n", &product.price);
	    
}

void billingInfo()
{
        printf("Order ID: ");
        //receive from server)
        printf("Customer ID: ");
        //receive from server);
        printf("Customer Address: ");
        //receive from server)
        printf("Total Order Price: ");
        //receive from server)
}


void customerOrder()
{
        printf("Order ID: ");
        //receive from server
        printf("Product ID: ");
        //receive from server
        printf("Quantity Purchased: ");
        //receive from server
        printf("Delivery Address: ");
        // receive from server
        printf("Total Price");
        //receive from server)
}
*/


int main(int argc, char const *argv[])
{
    
    int socketfd, port;
    int input, option, option1, option2, option3;
    struct sockaddr_in server_addr;
    struct hostent *server;
    

    //Tell the user to input the correct arguments in order
    if (argc < 3)
    {
        fprintf(stderr, "Please use the following arguments: %s Host/IP Port Number\n", argv[0]);
        exit(0);
    }
    //Converts charater string to an interger
    port = atoi(argv[2]);

    int connect_to_data_server(int socketfd) {

	socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(socketfd < 0) {
        printf("!!!Fatal error: socket cannot be created");
        exit(1);
    }

    // specify socket address
    int port;
    port = atoi(argv[2]);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
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

    /*
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket Failed");
        exit(0);
    }
    */
    printf("Socket Created\n");
/*
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {   
        fprintf(stderr, "Error");
    }
    //Connect to server using address
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr , (char *) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port);

    
    if(connect(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Failed Connection");
        exit(0);
    }
    printf("\nConnected\n");
*/
    while(1){
    

MAIN:
                printf ("Are you a customer or a seller? \n");
                printf ("1.Customer \n");
                printf ("2.Seller\n");
                printf ("3.Terminate\n");
                printf ("Choose an Option: ");
                scanf ("%d", &option);

                switch(option)
                {
                    case 1:;
                        goto CUSTOMERMENU;         
                    break;
                    
                    case 2:;
                        goto SELLERMENU;
                    break;

                    case 3:;
                        goto Exit;
                    break;

                    default:;
                        printf("Invalid Option");
                        
                }
                printf("\n");
    

CUSTOMERMENU:  
                printf("\n");

                printf ("Are you a New Customer or Returning Customer? \n");
                printf ("1.New Customer\n");
                printf ("2.Returning Customer\n"); // Figure out a function for returning customer
                printf ("3.Back to Customer or Seller \n");
                printf ("Choose an Option: ");
                scanf ("%d", &option1);

                switch(option1)
                {
                    case 1:;
                    //customerinfo function
			        customer_information_t customer;
                    char hello[45];

				    printf("\n"); //

                    customer.id = id();
                    printf("Customer Id: %d\n",customer.id);
                    fgets(hello,45,stdin);

                    fflush(stdout);
			        printf("Customer Name: ");
                    fgets(customer.name,BUFFER_SIZE,stdin);

                    printf("Customer Contact Number: ");
                    fgets(customer.contact_number,BUFFER_SIZE,stdin); 
                    
			        printf("Customer Address: ");
                    fgets(customer.contact_address,BUFFER_SIZE,stdin);
                    
                    printf("Hello, %s\n",customer.name);
                    printf("Hello, %s\n",customer.contact_number);

                    //send the customer information
                    send_new_customer(socketfd, customer);// add the header files for this
                    //goto BUY,return product, view product
                    break;

                    case 2:;
                        //ruturning customer function
                    break;

                    case 3:;
                        goto MAIN;
                    break;
                }
                
                
SELLERMENU:
                printf("\n");

                printf ("Are you a New Seller, returning seller or back to Main Menu?\n");
                printf ("1.New Seller\n");
                printf ("2.Returning Seller\n"); // Figure out a function for returning customer
                printf ("3.Add Product\n");
                printf ("4.Edit Product\n");
                printf ("Choose an Option:\n");
                scanf ("%d", &option2);

                switch(option2)
                {
                    case 1:;
                    seller_information_t seller;

                    char fake[45];

				    printf("\n"); //

                    seller.id = id();
                    printf("Seller Id: %d\n",seller.id);
                    fgets(fake,45,stdin);

                    fflush(stdout);
			        printf("Seller Name: ");
                    fgets(seller.name,BUFFER_SIZE,stdin);

                    printf("Seller Contact Number: ");
                    fgets(seller.contact_number,BUFFER_SIZE,stdin); 
                    
			        printf("Seller Address: ");
                    fgets(seller.contact_address,BUFFER_SIZE,stdin);
                    
                    printf("Hello, %s\n",seller.name);
                    printf("Hello, %s\n",seller.contact_number);
                    printf("Hello, %s\n",seller.contact_address);

                    send_new_seller(socketfd,seller);
                        //seller info function
                    break;;

                    case 2:

                        //returning seller function
                    break;;

                    case 3:;
                    product_information_t product;

                    char hello[45];

				    printf("\n"); //

                    product.id = id();
                    printf("Product Id: %d\n",product.id);
                    fgets(hello,45,stdin);
                    fflush(stdout);
			        printf("Product Description: ");
                    fgets(product.description,BUFFER_SIZE,stdin);

                    printf("Seller Id: ");
                    // fgets(product.seller_id,40,stdin); 
                    scanf("%d", &product.seller_id);
                
			        printf("Product Quantity Available: ");
                    //fgets(product.quantity,40,stdin);
                    scanf("%d",&product.quantity);

                    printf("Product Price: ");
                    //fgets(product.price,40,stdin);
                    scanf("%d",&product.price);

                    printf("Description, %s\n",product.description);
                    printf("Seller ID, %d\n",product.seller_id);
                    printf("Quantity, %d\n",product.quantity);
                    printf("Price, %d\n",product.price);

                    send_new_product(socketfd,product);


                        // Add Product
                    break;

                    case 4:;
                        // Edit Product
                    break;
                }
                printf("\n");

BUY:
                printf ("View, Buy or Return a Poduct(s)?\n");
                printf ("1.View Product(s)\n");
                printf ("2.Buy Product(s)\n"); // Figure out a function for returning customer
                printf ("3.Return Product(s)\n");
                printf ("4.Main Menu\n");
                printf ("Choose an Option:\n");
                scanf ("%d", &option3);

                switch(option3)
                {
                    case 1:;
                        // Receiveing all product information from the server
                    break;

                    case 2:;
                        // 
                    break;

                    case 3:;
                        // Receive Customer Order and then Ask customer which product to return
                    break;

                    case 4:;
                        goto MAIN;
                    break;
                }
        }
               
Exit:    close(socketfd);
         return 0;

    
}

int id()
{
    srand(time(NULL));
    int q = rand() % 2000 + 1;
    int *keys = &q;

    return q;
}
