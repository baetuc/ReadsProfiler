#ifndef READER_H
#define READER_H

#include<string>
#include <mainwindow.h>

using namespace std;

class Reader {
public:

    static string receiveMessageFromServer(int sd, MainWindow* parent);
    static void sendMessageToServer(int sd, MainWindow* parent, string message);
};

#endif // READER_H
