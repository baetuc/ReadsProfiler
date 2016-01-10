#include "Author.h"
#include <string>
#include <list>
#include <Creation.h>

using namespace std;

string Creation::getTitle() {return title;}
unsigned int Creation::getVolume() {return volume;}
list<Author> Creation::getAuthors() {return authors;}
list<string> Creation::getGenres() {return genres;}
list<string> Creation::getSubgenres() {return subgenres;}

void Creation::setTitle(string title) {this->title = title;}
void Creation::setVolume(unsigned int volume) {this->volume = volume;}
void Creation::setAuthors(list<Author> authors) {this->authors = authors;}
void Creation::setGenres(list<string> genres) {this->genres = genres;}
void Creation::setSubgenres(list<string> subgenres) {this->subgenres = subgenres;}
