#include <string>

#pragma once

using namespace std;

class User {
private:
    string username;
    string password;

public:
    string getUsername();
    string getPassword();
    void setUsername(string username);
    void setPassword(string password);
};
