#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
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

#define USER_PASSWORD_OK 'T'
#define WRONG_PASSWORD 'P'
#define WRONG_USERNAME 'U'

#define USER_EXISTS 'T'
#define USER_NO_EXISTS 'F'

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
    static void* execute(void* argument);
};

void Server::connect(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    uint32_t requestLength;
    if (read(client, &requestLength, sizeof(uint32_t)) <= 0) {
        printf("[Thread %d] Error at reading connect information from client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }
    string serializedUser;
    char letter;
    for(uint32_t i = 0; i < requestLength; ++i) {
        if (read(client, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at reading connect information from client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
        serializedUser += letter;
    }
    char response;
    // response will be 'T' if username and password are valid, 'P' if wrong
    // password and 'U' if wrong username
    User user = SerializerDeserializer::deserializeUser(serializedUser);
    string existingPassword;
    if(!DatabaseQueries::getPasswordForUser(user.getUsername(), existingPassword)) {
        // user does not exist
        response = WRONG_USERNAME;
    }
    else {
        if(existingPassword.compare(user.getPassword()) == 0) {
            response = USER_PASSWORD_OK;
        }
        else {
            response = WRONG_PASSWORD;
        }
    }

    if (write(client, &response, sizeof(char)) <= 0) {
        printf("[Thread %d] Error at writing connect response to client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }
}

void Server::createUser(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    uint32_t requestLength;
    if (read(client, &requestLength, sizeof(uint32_t)) <= 0) {
        printf("[Thread %d] Error at reading connect information from client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }

    string serializedUser;
    char letter;
    for(uint32_t i = 0; i < requestLength; ++i) {
        if (read(client, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at reading connect information from client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
        serializedUser += letter;
    }

    char response;
    // response will be 'T' if username doesn't already exists and 'F' in the other case
    User user = SerializerDeserializer::deserializeUser(serializedUser);
    if(!DatabaseQueries::createUser(user.getUsername(), user.getPassword())) {
        // created successfully
        response = USER_NO_EXISTS;
    }
    else {
        response = USER_EXISTS;
    }

    if (write(client, &response, sizeof(char)) <= 0) {
        printf("[Thread %d] Error at writing connect response to client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }
}

void Server::search(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    uint32_t requestLength;
    if (read(client, &requestLength, sizeof(uint32_t)) <= 0) {
        printf("[Thread %d] Error at reading connect information from client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }
    string serializedBook;
    char letter;
    for(uint32_t i = 0; i < requestLength; ++i) {
        if (read(client, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at reading connect information from client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
        serializedBook += letter;
    }
    Book book = SerializerDeserializer::deserializeBook(serializedBook);
    list<Book> response = DatabaseQueries::getResponseToQuery(book);
    string serializedBooks = SerializerDeserializer::serializeBookList(response);
    uint32_t responseLength = serializedBooks.size();

    if (write(client, &responseLength, sizeof(uint32_t)) <= 0) {
        printf("[Thread %d] Error at writing search response to client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }

    for(uint32_t i = 0; i < requestLength; ++i) {
        if (write(client, &serializedBooks[i], sizeof(char)) <= 0) {
            printf("[Thread %d] Error at writing search response to client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
    }
}

void Server::fetch(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    uint32_t requestLength;
    if (read(client, &requestLength, sizeof(uint32_t)) <= 0) {
        printf("[Thread %d] Error at reading connect information from client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }
    string ISBN;
    char letter;
    for(uint32_t i = 0; i < requestLength; ++i) {
        if (read(client, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at reading connect information from client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
        ISBN += letter;
    }
    string path = DatabaseQueries::getPath(ISBN);
    FILE * f = fopen(path.c_str(), "r");
    fseek(f, 0, SEEK_END);
    off_t fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fclose(f);
    int fd = open(path.c_str(), O_RDONLY, 0700);

    // The client must be careful here.
    if (write(client, &fileSize, sizeof(off_t)) <= 0) {
        printf("[Thread %d] Error at writing fetch response to client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }

    for(off_t i = 0; i < requestLength; ++i) {
        if(read(fd, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at reading information from file.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
        if (write(client, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at writing fetch response to client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
    }
    close(fd);

}

void Server::rate(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    uint32_t requestLength; // rating length
    if (read(client, &requestLength, sizeof(uint32_t)) <= 0) {
        printf("[Thread %d] Error at reading connect information from client.\n", threadId);
        //TODO: transmit error to client.. Maybe exceptions?
    }
    string serializedRating;
    char letter;
    for(uint32_t i = 0; i < requestLength; ++i) {
        if (read(client, &letter, sizeof(char)) <= 0) {
            printf("[Thread %d] Error at reading connect information from client.\n", threadId);
            //TODO: transmit error to client.. Maybe exceptions?
        }
        serializedRating += letter;
    }
    Rating rating = SerializerDeserializer::deserializeRating(serializedRating);
    DatabaseQueries::rateBook(rating.getRating(), rating.getISBN());
}

void Server::recommend(ThreadInformation* data) {

}

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
void* Server::execute(void* argument) {
    ThreadInformation* data = (ThreadInformation*) argument;
    printf ("[thread]- %d - Ready to communicate with the client.\n", data->getThreadId());
    fflush (stdout);
    pthread_detach(pthread_self());
    communicate(data);
    // by this point, we ended the communication with this client and close the connection
    close (data->getClient());
    return(NULL);
}
