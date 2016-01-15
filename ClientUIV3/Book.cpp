#include "Book.h"
#include <string>
#include <list>

using namespace std;

Creation Book::getCreation() {return creation;}
string Book::getISBN() {return ISBN;}
string Book::getPublisher() {return publisher;}
unsigned int Book::getPublicationYear() {return publicationYear;}
float Book::getRating() {return rating;}
string Book::getDescription() {return description;}

void Book::setCreation(Creation creation) {this->creation = creation;}
void Book::setISBN(string ISBN) {this->ISBN = ISBN;}
void Book::setPublisher(string publisher) {this->publisher = publisher;}
void Book::setPublicationYear(unsigned int publicationYear) {this->publicationYear = publicationYear;}
void Book::setRating(float rating) {this->rating = rating;}
void Book::setDescription(string description) {this->description = description;}
