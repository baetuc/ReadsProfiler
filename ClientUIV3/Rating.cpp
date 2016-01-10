#include <string>
#include <Rating.h>

using namespace std;

string Rating::getISBN() {return ISBN;}
int Rating::getRating() {return rating;}
void Rating::setISBN(string ISBN) {this->ISBN = ISBN;}
void Rating::setRating(int rating) {this->rating = rating;}
