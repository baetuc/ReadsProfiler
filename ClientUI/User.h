#include <string>

#pragma once

using namespace std;

class User {
private:
    string username;
    string password;

public:
    string getUsername() {return username;}
    string getPassword() {return password;}
    void setUsername(string username) {this->username=username;}
    void setPassword(string password) {this->password = password;}
};
