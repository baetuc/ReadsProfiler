#ifndef SOCKET_H
#define SOCKET_H

class Socket {
public:
    Socket();
    int sd;
    bool connected;

    void prepareCommunication();
    void closeConnection();
};

#endif // SOCKET_H
