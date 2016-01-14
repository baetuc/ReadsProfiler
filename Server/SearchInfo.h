#include <string>
#include "Book.h"
#pragma once

using namespace std;

class SearchInfo
{
private:
    string username;
    Book query;

public:
    string getUsername() {return username;}
    Book getQuery() {return query;}
    void setUsername(string username) {this->username = username;}
    void setQuery(Book query) {this->query = query;}
};
