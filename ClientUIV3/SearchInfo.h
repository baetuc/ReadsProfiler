#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include<string>
#include<Book.h>

using namespace std;

class SearchInfo
{
private:
    string username;
    Book query;

public:
    string getUsername();
    Book getQuery();
    void setUsername(string);
    void setQuery(Book);
};

#endif // SEARCHINFO_H
