#include <string>
#pragma once

using namespace std;

class Author {
private:
    string firstName;
    string secondName;

public:
    string getFirstName() ;
    string getSecondName();
    void setFirstName(string firstName);
    void setSecondName(string secondName);
};
