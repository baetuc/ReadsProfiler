#include "Author.h"
#include <string>
#include <list>
#pragma once

using namespace std;

class Creation {
private:
    string title;
    unsigned int volume;
    list<Author> authors;
    list<string> genres;
    list<string> subgenres;

public:
    string getTitle();
    unsigned int getVolume();
    list<Author> getAuthors();
    list<string> getGenres() ;
    list<string> getSubgenres();

    void setTitle(string title);
    void setVolume(unsigned int volume);
    void setAuthors(list<Author> authors);
    void setGenres(list<string> genres);
    void setSubgenres(list<string> subgenres);
};
