/* ************************************************************************************
 Author Name: Karan Bhakta                           Student ID: A20057064
 Email: kbhakta@okstate.edu
 Date: 10/2/2021
 Program Description: Main file for the Server Communication
* ********************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



int main(int argc, char *argv[])
{
    //Tell the user to input the correct arguments 
    if(argc < 2){
        printf("Port not valid");
        exit(1);
    }

    int socketfd, newsocketfd, port, input;
    int option1, option2;
    char buffer[1000];
    pid_t pid;

    struct sockaddr_in server_addr , client_addr;
    socklen_t clientlen;
    

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket Fialed");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    port = atoi(argv[1]);
    
    // Connect to server using address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    

    // Bind Socket with CLient
    if(bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    // Listen from Client
    listen(socketfd, 10);
    clientlen = sizeof(client_addr);
    
    

    // Accept incoming data from client
    if((newsocketfd = accept(socketfd, (struct sockaddr *) &client_addr, &clientlen)) < 0) // changed from client_addr
    {
       perror("Error, Not Accepted");
       exit(EXIT_FAILURE);
    }

    // Main Menu Options
        
Menu:;   input = write(newsocketfd,"Main Menu \n[1] Single Player Mode \n[2] Mutiplayer Mode \n[3] Exit\n", strlen("Main Menu \n[1] Single Player Mode \n[2] Mutiplayer Mode \n[3] Exit\n"));
        read(newsocketfd, &option1, sizeof(int));

        printf("Please choose an option: %d\n ", option1);

        switch(option1)
        {
            case 1:
                sleep(15);
                goto Menu;
                break;

            case 2:
                goto Multi;
                break;

            case 3:
                goto Exit;
                break;
            
        }
    
        // Multiplayer Switch function
Multi:  input = write(newsocketfd,"Multiplayer Main Menu \n[1] Enter Player Information \n[2] Initialize The Board \n[3] Populate The Board \n[4] Play The Game \n[5] Score Board \n[6] Back to Main Menu\n", strlen("Multiplayer Main Menu \n[1] Enter Player Information \n[2] Initialize The Board \n[3] Populate The Board \n[4] Play The Game \n[5] Score Board \n[6] Back to Main Menu\n"));
        read(newsocketfd, &option2, sizeof(int));

        printf("Please choose an option: %d\n ", option2);

        switch(option1)
        {
            case 1:
            //Playerinfo
            goto Multi;
            break;

            case 2:
            //initialize game board
            break;

            case 3:
            //Populate Board
            break;

            case 4:
            //Play the Game
            break;

            case 5:
            //Score Board
            break;

            case 6:
            goto Menu;
            break;
        
        }

        if(option2 != 7)
            {
                goto Multi;
            } 

        // Close the sockets and connection
Exit:   close(socketfd);
        close(newsocketfd);
        return 0;
        
}