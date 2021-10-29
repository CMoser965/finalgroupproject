/**
 * Author:  Christian Moser
 * Date:    10-28-21
 * Description: Driver file for data server.
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

