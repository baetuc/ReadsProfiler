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
    string description;

public:
    Creation getCreation();
    string getISBN();
    string getPublisher();
    unsigned int getPublicationYear();
    float getRating();
    string getDescription();

    void setCreation(Creation creation);
    void setISBN(string ISBN);
    void setPublisher(string publisher);
    void setPublicationYear(unsigned int publicationYear);
    void setRating(float rating);
    void setDescription(string);
};
