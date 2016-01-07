#include <Socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

using namespace std;

#define PORT 3002

Socket::Socket() {
    connected = false;
}

void Socket::prepareCommunication() {
    if(connected) {
        return;
    }
    struct sockaddr_in server;

    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
      {
        throw "Eroare la crearea socketului.\n";
      }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons (PORT);

    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
      {
        throw "Eroare la conectarea cu serverul.\n";
      }

    connected = true;
}

void Socket::closeConnection() {
    close(sd);
}
