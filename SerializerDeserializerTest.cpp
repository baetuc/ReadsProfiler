#include "SerializerDeserializer.h"
#include "Book.h"
#include "Author.h"
#include <string>
#include <list>

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
    author.setSecondName("Mirciulet");
    authors.push_back(author);author.setFirstName("Liviu");
    author.setSecondName("Hrusca");
    authors.push_back(author);

    Creation creation;
    creation.setAuthors(authors);
    creation.setTitle("Best book ever.");
    creation.setVolume(2);

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

    string serializedBook;
    serializedBook = serializer.serializeBook(book);
    cout << serializedBook << '\n';
    Book otherBook = serializer.deserializeBook(serializedBook);

    cout << "The book has the following properties:\n";
    cout << "Title: " << otherBook.getCreation().getTitle() << '\n';
    cout << "Volume: " << otherBook.getCreation().getVolume() << '\n';
    list<Author> curAuthors = otherBook.getCreation().getAuthors();
    cout << "Authors: ";
    for(list<Author>::iterator it = curAuthors.begin(); it != curAuthors.end(); ++it) {
        cout << it->getFirstName() << ' ' << it->getSecondName() << ", ";
    }
    cout << '\n';
    cout << "Genres: ";
    list<string> genress = otherBook.getCreation().getGenres();
    for(list<string>::iterator it = genress.begin(); it != genress.end(); ++it) {
        cout << *it << ", ";
    }
    cout << '\n';

    list<string> subgenress = otherBook.getCreation().getSubgenres();
    cout << "Subgenres: ";
    for(list<string>::iterator it = subgenress.begin(); it != subgenress.end(); ++it) {
        cout << *it << ", ";
    }

    cout << '\n';

    cout << "ISBN: " << otherBook.getISBN() << '\n';
    cout << "Publisher: " << otherBook.getPublisher() << '\n';
    cout << "Aparition year: " << otherBook.getPublicationYear() << '\n';
    cout << "Rating: " << otherBook.getRating() << '\n';
}
