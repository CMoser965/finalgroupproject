/**
 * Author:  Christian Moser
 * Date:    11-01-21
 * Description: Test client for database server.
 * **/

// standard libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// threading libs
#include <pthread.h>
#include <signal.h>

// sys call libs
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

// net standard libs
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "include/data.h"

// int main() {
//     int cliconn = init_client(PORT);
//     CustomerInfo customer = (CustomerInfo)receive(cliconn, CUSTOMER);
//     printf("Name: %s\nID: $d\nPhone #: %d\nAddress: %s\n", customer.name, customer.id, customer.contact_number, customer.contact_address);
//     return 0;
// }