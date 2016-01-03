/*
    Author: Ciprian Baetu
    Date: 03 Jan 2016
    Source: Ms. Professor Lenuta Alboaie courses
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <vector>
#include "Server.h"

#define PORT 3002
extern int errno;

using namespace std;

int main ()
{
    struct sockaddr_in server;
    struct sockaddr_in from;
    int socketDescriptor;
    vector<pthread_t*> threads;
    int numberOfThreads = 0;


    // Socket creation, for TCP communication
    if ((socketDescriptor = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        // TODO : CREATE A LOG SYSTEM
        perror ("Error at creating socket.\n");
        return errno;
    }
    // using SO_REUSEADDR option
    int on = 1;
    setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    // preparing the data structures for server and client
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    // we bind the address to the socket
    if (bind (socketDescriptor, (struct sockaddr*) &server, sizeof (struct sockaddr)) == -1)
    {
        perror ("Error at binding address to server.\n");
        return errno;
    }

    // server listens for communication requests
    if (listen (socketDescriptor, 5) == -1)
    {
        perror ("Error at listening for requests.\n");
        return errno;
    }

    while (1)
    {
        int client;
        // parameter for the function executed by the thread
        ThreadInformation* data = new ThreadInformation;
        int addressLength = sizeof (from);

        printf ("Waiting at port: %d...\n", PORT);
        fflush (stdout);

        if ((client = accept(socketDescriptor, (struct sockaddr*) &from, &addressLength)) < 0)
        {
            perror ("Error at accepting client.\n");
            continue;
        }

        data->setThreadId(++numberOfThreads);
        data->setClient(client);
        // We create a new thread in the vector
        threads.push_back(new pthread_t); // This may be tricky


        pthread_create(threads[i - 1], NULL, &Server::execute, data);

    }
    return 0;
}
