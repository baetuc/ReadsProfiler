#include <string>

#pragma once

class Rating {
private:
    string ISBN;
    int rating;

public:
    string getISBN() {return ISBN;}
    int getRating() {return rating;}
    void setISBN(string ISBN) {this->ISBN = ISBN;}
    void setRating(int rating) {this->rating = rating;}
};
