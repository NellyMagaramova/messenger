#include "common_header.h"
#define PREFIX "SERVER: "
#define MAX_BUFFER 4095
#define PORT 22000
#include <arpa/inet.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    char str[MAX_BUFFER];
    /*socket for exchange data (send/recv)*/
    int client1 = -1;
    int client2 = -1;

    /*a descripter socket for binding to an adress */
    int listener;

    struct sockaddr_in servaddr;

    printf("\n   sozdaem ochered zaprosov na soedinenie \n");
    listener = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT); /* notice host to network conversion */

    printf("Bind socket \n");
    bind(listener, (struct sockaddr *) &servaddr, sizeof(servaddr));

    printf("Start listen on the socket\n");
    listen(listener, MAX);



    fd_set active_fd_set, read_fd_set;
    int i;
    struct sockaddr_in clientname;
    size_t size;

    /* Initialize the set of active sockets. */
    FD_ZERO (&active_fd_set);
    FD_SET (listener, &active_fd_set);

    while (1)
    {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            perror ("select");
            exit (EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET (i, &read_fd_set))
            {
                if (i == listener)
                {
                    if(client1 >= 0) {
                        printf("Pending connection from client 2\n");
                        client2 = accept(listener, 0, 0);
                        printf("Accepted connection %d\n", client2);
                        FD_SET (client2, &active_fd_set);
                    } else {
                        printf("Pending connection from client 1\n");
                        client1 = accept(listener, 0, 0);
                        printf("Accepted connection %d\n", client1);
                        FD_SET (client1, &active_fd_set);
                    }
                    /* Connection request on original socket. */

                }
                else
                {
                    int readData = read(i, str, MAX_BUFFER);
                    if (readData <= 0)
                    {
                        close (i);
                        FD_CLR (i, &active_fd_set);
                        if(i == client1) {
                            client1 = -1;
                        } else if(i == client2){
                            client2 = -1;
                        }
                        continue;

                    }

                    printf("Data from: %d %d %d\n", client1, client2, i);

                    if(i == client1) {
                        if(client2 > 0) {
                            write(client2, str, readData);
                        } else {
                            write(client1, str, readData);
                        }
                    } else {
                        if(client1 > 0) {
                            write(client1, str, readData);
                        } else {
                            write(client2, str, readData);
                        }
                    }
                }
            }
    }
}


