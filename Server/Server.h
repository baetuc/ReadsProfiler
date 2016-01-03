#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include "Book.h"
#include "DatabaseQueries.h"
#include "SerializerDeserializer.h"
#include "ThreadInformation.h"

#define DISCONNECT 0
#define CONNECT 1
#define CREATE_USER 2
#define SEARCH 3
#define FETCH 4
#define RATE 5
#define RECOMMEND 6

using namespace std;

class Server {
private:

    static void connect(ThreadInformation* data);
    static void createUser(ThreadInformation* data);
    static void search(ThreadInformation* data);
    static void fetch(ThreadInformation* data);
    static void rate(ThreadInformation* data);
    static void recommend(ThreadInformation* data);
    static void communicate(ThreadInformation* data);
public:
    static void execute(void* argument);
};

void Server::communicate(ThreadInformation* data){
    int client = data->getClient();
    int threadId = data->getThreadId();
    uint32_t requestType; // this will hold the request type

    if (read(client, &requestType, sizeof(uint32_t)) <= 0) {
        printf("Error at reading information from client.\n");
        //TODO: transmit error to client.. Maybe exceptions?
    }

    while(requestType != DISCONNECT) {
        switch(requestType) {
            case CONNECT:
                        connect(data);
                        break;
            case CREATE_USER:
                        createUser(data);
                        break;
            case SEARCH:
                        search(data);
                        break;
            case FETCH:
                        fetch(data);
                        break;
            case RATE:
                        rate(data);
                        break;
            case RECOMMEND:
                        recommend(data);
                        break;
        }
        // Read again the request type, in a loop, while disconnect request
        if (read(client, &requestType, sizeof(uint32_t)) <= 0) {
            printf("Error at reading information from client.\n");
            //TODO: transmit error to client.. Maybe exceptions?
        }
    }
}

// the argument taken is a pointer to ThreadInformation
void Server::execute(void* argument) {
    ThreadInformation* data = (ThreadInformation*) argument;
    printf ("[thread]- %d - Ready to communicate with the client.\n", data->getThreadId());
    fflush (stdout);
    pthread_detach(pthread_self());
    communicate(data);
    // by this point, we ended the communication with this client and close the connection
    close (data->getClient());
}
