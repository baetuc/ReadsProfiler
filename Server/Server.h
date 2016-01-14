#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "Book.h"
#include <fstream>
#include "DatabaseQueries.h"
#include "SerializerDeserializer.h"
#include "ThreadInformation.h"
#include "Recommender.h"

#define ERR_MSG 0
#define OK_MSG 1

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
    static void sendMessageToClient(int threadId, int client, string message, bool error);
    static string receiveMessageFromClient(int threadId, int client);
    static void* execute(void* argument);
};

void Server::sendMessageToClient(int threadId, int client, string message, bool error) {
    uint32_t typeOfMessage = error == true ? ERR_MSG : OK_MSG;
    cout << typeOfMessage << '\n';
    typeOfMessage = htonl(typeOfMessage);

    if(write(client, &typeOfMessage, sizeof(uint32_t)) < 0) {
        string err = "[Thread ";
        err += Utility::getStringForNumber(threadId);
        err += "]: Nu am putut trimite tipul mesajului la client:\n";
        err += message;
        const char* charErr = err.c_str();
        throw charErr;
    }
    uint32_t size = message.size();
    cout << size << '\n';
    cout << message << '\n';
    size = htonl(size);
    if(write(client, &size, sizeof(uint32_t)) < 0) {
        string err = "[Thread ";
        err += Utility::getStringForNumber(threadId);
        err += "]: Nu am putut trimite lungimea mesajului la client:\n";
        err += message;
        const char* charErr = err.c_str();
        throw charErr;
    }
    size = message.size();
    cout << size << '\n';
    for(uint32_t i = 0; i < size; ++i) {
        //cout << "Caracterul " << i << "\n";
        if (write(client, &message[i], sizeof(char)) <= 0) {
            string err = "[Thread ";
            err += Utility::getStringForNumber(threadId);
            err += "] Eroare la scriere mesaj catre client:\n";
            err += message;
            const char* charErr = err.c_str();
            throw charErr;
        }
    }
}

string Server::receiveMessageFromClient(int threadId, int client) {
    string response;
    char letter;
    uint32_t size;
    if(read(client, &size, sizeof(uint32_t)) < 0) {
        string err = "[Thread ";
        err += Utility::getStringForNumber(threadId);
        err += "] Eroare la citirea lungimii mesajului de la client:\n";
        const char* charErr = err.c_str();
        throw charErr;
    }
    size = ntohl(size);
    for(uint32_t i = 0; i < size; ++i) {
        if(read(client, &letter, sizeof(char)) < 0) {
            string err = "[Thread ";
            err += Utility::getStringForNumber(threadId);
            err += "] Eroare la citirea mesajului de la client:\n";
            const char* charErr = err.c_str();
            throw charErr;
        }
        response += letter;
    }
    return response;
}

void Server::connect(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();
    string serializedUser = receiveMessageFromClient(threadId, client);
    string response;
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

    sendMessageToClient(threadId, client, response, false);
}

void Server::createUser(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    string serializedUser = receiveMessageFromClient(threadId, client);

    string response;
    // response will be 'T' if username doesn't already exists and 'F' in the other case
    User user = SerializerDeserializer::deserializeUser(serializedUser);
    if(!DatabaseQueries::createUser(user.getUsername(), user.getPassword())) {
        // created successfully
        response = USER_NO_EXISTS;
    }
    else {
        response = USER_EXISTS;
    }

    sendMessageToClient(threadId, client, response, false);
}

void Server::search(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    string serializedSearch = receiveMessageFromClient(threadId, client);
    cout << serializedSearch << '\n';
    SearchInfo search = SerializerDeserializer::deserializeSearch(serializedSearch);
    list<Book> response = DatabaseQueries::getResponseToQuery(search.getQuery());
    DatabaseQueries::saveQuery(search);
    string serializedBooks = SerializerDeserializer::serializeBookList(response);
    sendMessageToClient(threadId, client, serializedBooks, false);
}

void Server::fetch(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    string ISBN = receiveMessageFromClient(threadId, client);
    //cout << "Received ISBN: " << ISBN << '\n';
    string path = DatabaseQueries::getPath(ISBN);
    //cout << "Path: " << path << '\n';
    cout << "Path is: " << path << " a\n";

    //uint32_t fileSize = 0;
    uint32_t fileSize = 0;
    ifstream fileSeqIn;
    fileSeqIn.open(path.c_str(), ios::in | ios::binary |ios::ate);
    fileSize = fileSeqIn.tellg();
    fileSeqIn.close();

    fileSize = fileSize;
    if(fileSize < 0) {
        string err = "[Thread ";
        err += Utility::getStringForNumber(threadId);
        err += "] Eroare la aflarea dimensiunii fisierului.\n";
        const char* charErr = err.c_str();
        throw charErr;
    }
    cout << fileSize << '\n';

    int fd = open(path.c_str(), O_RDONLY, 0700);

    fileSize = htonl(fileSize);
    // The client must be careful here.
    if (write(client, &fileSize, sizeof(uint32_t)) <= 0) {
        string err = "[Thread ";
        err += Utility::getStringForNumber(threadId);
        err += "] Eroare la scrierea raspunsului de download a fisierului.\n";
        const char* charErr = err.c_str();
        throw charErr;
    }
    fileSize = ntohl (fileSize);
    char letter;

    for(off_t i = 0; i < fileSize; ++i) {
        if(read(fd, &letter, sizeof(char)) <= 0) {
            string err = "[Thread ";
            err += Utility::getStringForNumber(threadId);
            err += "] Eroare la citirea informatiei din fisier.\n";
            const char* charErr = err.c_str();
            throw charErr;
        }
        if (write(client, &letter, sizeof(char)) <= 0) {
            string err = "[Thread ";
            err += Utility::getStringForNumber(threadId);
            err += "] Eroare la scrierea fisierului catre client.\n";
            const char* charErr = err.c_str();
            throw charErr;
        }
    }
    close(fd);
}

void Server::rate(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();

    string serializedRating = receiveMessageFromClient(threadId, client);
    cout << "Rating: " << serializedRating << '\n';
    Rating rating = SerializerDeserializer::deserializeRating(serializedRating);
    cout << "Rate received: " << rating.getRating() << '\n';
    string response = DatabaseQueries::rateBook(rating.getRating(), rating.getISBN(), rating.getUsername());
    sendMessageToClient(threadId, client, response, false);
}

void Server::recommend(ThreadInformation* data) {
    int client = data->getClient();
    int threadId = data->getThreadId();
    string username = receiveMessageFromClient(threadId, client);
    cout << "Username: " << username << '\n';
    Recommender recommender(username);
    list<string> recommendedISBNs = recommender.recommendBooks();
    list<Book> result = DatabaseQueries::getBooksFromISBNList(recommendedISBNs);
    string serializedBooks = SerializerDeserializer::serializeBookList(result);
    sendMessageToClient(threadId, client, serializedBooks, false);
}

void Server::communicate(ThreadInformation* data){
    int client = data->getClient();
    int threadId = data->getThreadId();
    uint32_t requestType; // this will hold the request type
    if (read(client, &requestType, sizeof(uint32_t)) <= 0) {
        string err = "[Thread ";
        err += Utility::getStringForNumber(threadId);
        err += "] Eroare la citirea informatiei de la client.\n";
        const char* charErr = err.c_str();
        throw charErr;
    }
    requestType = ntohl(requestType);

    while(requestType != DISCONNECT) {
        cout << requestType << '\n';
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
            string err = "[Thread ";
            err += Utility::getStringForNumber(threadId);
            err += "] Eroare la citirea informatiei de la client.\n";
            const char* charErr = err.c_str();
            throw charErr;
        }
        requestType = ntohl(requestType);
    }
}

// the argument taken is a pointer to ThreadInformation
void* Server::execute(void* argument) {
    ThreadInformation* data = (ThreadInformation*) argument;
    printf ("[thread]- %d - Ready to communicate with the client.\n", data->getThreadId());
    fflush (stdout);
    pthread_detach(pthread_self());
    try {
        communicate(data);
    }
    catch(const char* message) {
        cout << message;
        /*try {
            sendMessageToClient(data->getThreadId(), data->getClient(), message, true);
        }
        catch(const char* message) {
            cout << "Nu am putut comunica eroarea clientului.\n";
        }*/
    }

    // by this point, we ended the communication with this client and close the connection
    close (data->getClient());
    return(NULL);
}
