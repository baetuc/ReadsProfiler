
#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<QMessageBox>
#include <Reader.h>
#include <mainwindow.h>
#include<Utility.h>


#define ERR_MSG 0
#define OK_MSG 1

using namespace std;

string Reader::receiveMessageFromServer(int sd, MainWindow* parent) {
    uint32_t typeOfMessage;
    if(read(sd, &typeOfMessage, sizeof(uint32_t)) < 0) {
        QMessageBox::warning(parent, "WARNING", "Eroare la citirea tipului de mesaj de la server!");
        exit(0);
    }
    typeOfMessage = ntohl(typeOfMessage);
    bool error = typeOfMessage == ERR_MSG ? true : false;

    uint32_t size;
    if(read(sd, &size, sizeof(uint32_t)) < 0) {
        QMessageBox::warning(parent, "WARNING", "Eroare la citirea lungimii mesajului de la server!");
        exit(0);
    }

    size = ntohl(size);
    char letter;
    string message;
    for(uint32_t i = 0; i < size; ++i) {
        if (read(sd, &letter, sizeof(char)) <= 0) {
            QMessageBox::warning(parent, "WARNING", "Eroare la citirea mesajului de la server!");
            exit(0);
        }
        message += letter;
    }

    if(error) {
        QMessageBox::warning(parent, "Eroare la server", message.c_str());
        exit(0);
    }
    return message;
}

void Reader::sendMessageToServer(int sd, MainWindow* parent, string message) {
    uint32_t size = message.size();
    size = htonl(size);
    if(write(sd, &size, sizeof(uint32_t)) < 0) {
        QMessageBox::warning(parent, "WARNING", "Eroare la scrierea lungimii mesajului catre server!");
        exit(0);
    }
    size = ntohl(size);
    for(uint32_t i = 0; i < size; ++i) {
        if(write(sd, &message[i], sizeof(char)) < 0) {
            QMessageBox::warning(parent, "WARNING", "Eroare la scrierea mesajului catre server!");
            exit(0);
        }
    }
}


