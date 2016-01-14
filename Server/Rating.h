#include <string>

#pragma once

class Rating {
private:
    string username;
    string ISBN;
    int rating;


public:
    string getISBN() {return ISBN;}
    int getRating() {return rating;}
    string getUsername() {return username;}
    void setISBN(string ISBN) {this->ISBN = ISBN;}
    void setRating(int rating) {this->rating = rating;}
    void setUsername(string username) {this->username = username;}
};
