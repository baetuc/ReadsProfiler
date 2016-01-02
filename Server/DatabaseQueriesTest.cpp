#include "DatabaseQueries.h"
#include <iostream>
#include "Book.h"

using namespace std;

int main() {
    Book book;
    list<Author> authors;
    Author author;
    author.setFirstName("Marcel");
    author.setSecondName("Ghenea");
    authors.push_back(author);
    author.setFirstName("Toader");
    author.setFirstName("");
    authors.push_back(author);author.setFirstName("Liviu");
    author.setSecondName("Hrusca");
    authors.push_back(author);

    Creation creation;
    creation.setAuthors(authors);
    //creation.setTitle("Best book ever.");
    creation.setVolume(0);

    list<string> genres;
    genres.push_back("Fictional");
    creation.setGenres(genres);

    list<string> subgenres;
    subgenres.push_back("Romantic");
    subgenres.push_back("History");
    creation.setSubgenres(subgenres);

    book.setCreation(creation);

    book.setISBN("194324324543342");
    book.setPublisher("Tabitha");
    book.setPublicationYear(2005);
    book.setRating(9.34);
    book.setPath("/home/cip/AllBooks/BestBookEver.txt");

    Book otherBook;
    Creation otherCreation = otherBook.getCreation();
    otherCreation.setVolume(0);
    otherBook.setCreation(otherCreation);

    cout << DatabaseQueries::getStatementForISBN(book) << '\n';
    cout << DatabaseQueries::getStatementForISBN(otherBook) << '\n';

}
