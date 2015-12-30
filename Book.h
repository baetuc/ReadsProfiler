#include "Creation.h"
#include <string>
#include <list>

using namespace std;

class Book {
private:
    Creation creation;
    string ISBN;
    string publisher;
    unsigned int publicationYear;
    double rating;
    string path;

public:
    Creation getCreation() {return creation;}
    string getISBN() {return ISBN;}
    string getPublisher() {return publisher;}
    unsigned int getPublicationYear() {return publicationYear;}
    double getRating() {return rating;}
    string getPath() {return path;}

    void setCreation(Creation creation) {this.creation = creation;}
    void setISBN(string ISBN) {this.ISBN = ISBN;}
    void setPublisher(string publisher) {this.publisher = publisher;}
    void setPublicationYear(unsigned int publicationYear) {this.publicationYear = publicationYear;}
    void setRating(double rating) {this.rating = rating;}
    void setPath(string path) {this.path = path;}
}
