#include "Creation.h"
#include <string>
#include <list>
#pragma once

using namespace std;

class Book {
private:
    Creation creation;
    string ISBN;
    string publisher;
    unsigned int publicationYear;
    float rating;

public:
    Creation getCreation() {return creation;}
    string getISBN() {return ISBN;}
    string getPublisher() {return publisher;}
    unsigned int getPublicationYear() {return publicationYear;}
    float getRating() {return rating;}

    void setCreation(Creation creation) {this->creation = creation;}
    void setISBN(string ISBN) {this->ISBN = ISBN;}
    void setPublisher(string publisher) {this->publisher = publisher;}
    void setPublicationYear(unsigned int publicationYear) {this->publicationYear = publicationYear;}
    void setRating(float rating) {this->rating = rating;}
};
