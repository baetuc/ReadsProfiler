#include <string>
#pragma once

using namespace std;

class BookScore {
    string book;
    double score;

public:
    BookScore(string book, double score) {this->book = book; this->score = score;}
    string getBook() {return book;}
    double getScore() {return score;}
    void setBook(string book) {this->book = book;}
    void setScore(double score) {this->score = score;}
    static bool comparator(BookScore first, BookScore second) { return first.getScore() > second.getScore();}
};
