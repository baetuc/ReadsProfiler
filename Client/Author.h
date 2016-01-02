#include <string>
#pragma once

using namespace std;

class Author {
private:
    string firstName;
    string secondName;

public:
    string getFirstName() {return firstName;}
    string getSecondName() {return secondName;}
    void setFirstName(string firstName) {this->firstName = firstName;}
    void setSecondName(string secondName) {this->secondName = secondName;}
};
