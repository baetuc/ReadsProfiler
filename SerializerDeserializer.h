#include "Book.h"
#include <string>
#include "stdio.h"
#include "stdlib.h"

using namespace std;

class SerializerDeserializer {
private:
    string serializeCreation(Creation);
    string serializeAuthor(Author);
    Creation deserializeCreation(string);
    Author deserializeAuthor(string);
public:
    string serializeBook(Book);
    Book deserializeBook(string);
}

string SerializerDeserializer::serializeAuthor(Author author) {
    serializedAuthor = author.getFirstName();
    serializedAuthor += ',';
    serializedAuthor += author.getSecondName();
    return serializedAuthor;
}

Author SerializerDeserializer::deserializeAuthor(string serializedAuthor) {
    Author author;
    string name;
    name.clear();
    int i = 0;
    while(serializedAuthor[i++] != ',') {
        name += serializedAuthor[i];
    }
    author.setFirstName(name);
    name.clear();
    while(i < serializedAuthor.size()) {
        name += serializedAuthor[i];
    }
    author.setSecondName(name);
    return author;
}

string SerializerDeserializer::serializedCreation(Creation creation) {
    string serializedCreation = "";
    char titleLength[4];
    itoa(creation.getTitle().size(), titleLength, 10);
    string titleLgth(titleLength);
    serializedCreation += titleLgth;
    // TODO: serialize creation further!!

    string serializedCreation = creation.getTitle();
    string volume;
    char creationVolume[4];

}

string SerializerDeserializer::serializeBook(Book book) {
    string serializedBook = "";
    string serializedCreation = serializeCreation(book.creation);
    serializedBook += serializedCreation;

}
