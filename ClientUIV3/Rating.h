#include <string>
#pragma once

using namespace std;

class Rating {
private:
    string ISBN;
    int rating;

public:
    string getISBN();
    int getRating();
    void setISBN(string ISBN);
    void setRating(int rating);
};
