#include "DatabaseQueries.h"
#include "SerializerDeserializer.h"
#include <iostream>
#include "Book.h"

using namespace std;

int main() {
    SerializerDeserializer serializer;
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

    Book otherBook;
    Creation otherCreation = otherBook.getCreation();
    otherCreation.setVolume(0);

    Author otherAuthor;
    list<Author> otherAuthors;
    otherAuthor.setFirstName("Tolstoi");
    //otherAuthor.setSecondName("Jack");
    otherAuthors.push_back(otherAuthor);
    otherCreation.setAuthors(otherAuthors);

    otherBook.setCreation(otherCreation);




    //cout << DatabaseQueries::getStatementForISBN(book) << '\n';
    //cout << DatabaseQueries::getStatementForISBN(otherBook) << '\n';

    list<Book> queryResult = DatabaseQueries::getResponseToQuery(otherBook);
    for(list<Book>::iterator it = queryResult.begin(); it != queryResult.end(); ++it) {
        cout << serializer.serializeBook(*it) << '\n';
    }

    return 0;

}
