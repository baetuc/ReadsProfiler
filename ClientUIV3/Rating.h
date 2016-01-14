#include <string>
#pragma once

using namespace std;

class Rating {
private:
    string ISBN;
    string username;
    int rating;

public:
    string getISBN();
    string getUsername();
    int getRating();
    void setISBN(string ISBN);
    void setRating(int rating);
    void setUsername(string);
};
