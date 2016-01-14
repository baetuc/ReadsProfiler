#include <string>
#include <Rating.h>

using namespace std;

string Rating::getISBN() {return ISBN;}
string Rating::getUsername() {return username;}
int Rating::getRating() {return rating;}

void Rating::setISBN(string ISBN) {this->ISBN = ISBN;}
void Rating::setUsername(string username) {this->username = username;}
void Rating::setRating(int rating) {this->rating = rating;}
