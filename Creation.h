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
    string getTitle() {return title;}
    unsigned int getVolume() {return volume;}
    list<Author> getAuthors() {return authors;}
    list<string> getGenres() {return genres;}
    list<string> getSubgenres() {return subgenres;}

    void setTitle(string title) {this->title = title;}
    void setVolume(unsigned int volume) {this->volume = volume;}
    void setAuthors(list<Author> authors) {this->authors = authors;}
    void setGenres(list<string> genres) {this->genres = genres;}
    void setSubgenres(list<string> subgenres) {this->subgenres = subgenres;}
};
