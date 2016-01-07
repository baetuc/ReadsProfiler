/**
    Author: Ciprian Baetu
    Date: 31 Dec 2015
*/

#include <string>
#include <vector>
#include <list>
#include <stdlib.h>
#include <iostream>
#include "Book.h"
#include "User.h"
#include "Rating.h"
#include "Utility.h"

using namespace std;

class SerializerDeserializer {
private:
    static string serializeCreation(Creation);
    static string serializeAuthor(Author);
    static Creation deserializeCreation(string);
    static Author deserializeAuthor(string);
public:
    static string serializeBook(Book);
    static Book deserializeBook(string);
    static string serializeBookList(list<Book>);
    static list<Book> deserializeBookList(string);
    static string serializeUser(User);
    static User deserializeUser(string);
    static string serializeRating(Rating);
    static Rating deserializeRating(string);
};

string SerializerDeserializer::serializeAuthor(Author author) {
    string serializedAuthor = author.getFirstName();
    serializedAuthor += ',';
    serializedAuthor += author.getSecondName();
    return serializedAuthor;
}

Author SerializerDeserializer::deserializeAuthor(string serializedAuthor) {
    Author author;
    vector<string> names = Utility::splitString(serializedAuthor, ',', 2);
    author.setFirstName(names[0]);
    author.setSecondName(names[1]);
    return author;
}

string SerializerDeserializer::serializeCreation(Creation creation) {
    // first, we write the title length and the title
    string serializedCreation = Utility::getStringForNumber((int)creation.getTitle().size());
    serializedCreation += ','; // because the number cannot contain commas
    serializedCreation += creation.getTitle();

    // then, we will write down the volume
    serializedCreation += Utility::getStringForNumber((int)creation.getVolume());
    serializedCreation += ','; // a number cannot contains commas

    // then, we will write the number of the authors and after that, the
    // serialized authors

    serializedCreation += Utility::getStringForNumber((int) creation.getAuthors().size());
    serializedCreation += ',';
    list<Author> authors = creation.getAuthors();
    for(list<Author>::iterator it = authors.begin(); it != authors.end(); ++it) {
        serializedCreation += serializeAuthor(*it);
        serializedCreation += ';';
    }
    // we write the number of genres, then the genres

    list<string> genres = creation.getGenres();
    serializedCreation += Utility::getStringForNumber((int) genres.size());
    serializedCreation += ',';
    for(list<string>:: iterator it = genres.begin(); it != genres.end(); ++it) {
        serializedCreation += (*it);
        serializedCreation += ',';
    }

    // we write the number of subgenres, then the subgenres

    list<string> subgenres = creation.getSubgenres();
    serializedCreation += Utility::getStringForNumber((int) subgenres.size());
    serializedCreation += ',';
    for(list<string>:: iterator it = subgenres.begin(); it != subgenres.end(); ++it) {
        serializedCreation += (*it);
        serializedCreation += ',';
    }

    return serializedCreation;

}

Creation SerializerDeserializer::deserializeCreation(string serializedCreation) {
    Creation creation;
    string current; // We will hold current substring in this variable
    vector<string> substrings = Utility::splitString(serializedCreation, ',', 2);
    // In first substring, we have the title length

    int titleLength = atoi(substrings[0].c_str());
    current = substrings[1];
    creation.setTitle(current.substr(0, titleLength));
    current = current.substr(titleLength, string::npos);

    // Now, we have to split by comma, to find out the volume
    substrings = Utility::splitString(current, ',', 2);
    creation.setVolume((unsigned int) atoi(substrings[0].c_str()));
    current = substrings[1];

    // We have to split by comma again, to find the number of authors
    substrings = Utility::splitString(current, ',', 2);
    int authorsNumber = atoi(substrings[0].c_str());
    current = substrings[1];

    // Now we split by semicolon, to determine the authors.
    substrings = Utility::splitString(current, ';', authorsNumber + 1);
    list<Author> authors;
    for(int i = 0; i < authorsNumber && i < substrings.size(); ++i) {
        Author author = deserializeAuthor(substrings[i]);
        authors.push_back(author);
    }
    creation.setAuthors(authors);
    current = substrings[authorsNumber];

    // Now, we split by comma, to find the number of genres
    substrings = Utility::splitString(current, ',', 2);
    int numberOfGenres = atoi(substrings[0].c_str());
    list<string> genres;
    substrings = Utility::splitString(substrings[1], ',', numberOfGenres +1);
    for(int i = 0; i < numberOfGenres && i < substrings.size(); ++i) {
        genres.push_back(substrings[i]);
    }
    creation.setGenres(genres);
    current = substrings[numberOfGenres];

    // Now we split by comma, to find the number of subgenres
    substrings = Utility::splitString(current, ',', 2);
    int numberOfSubgenres = atoi(substrings[0].c_str());
    list<string> subgenres;
    substrings = Utility::splitString(substrings[1], ',', numberOfSubgenres +1);
    for(int i = 0; i < numberOfSubgenres && i < substrings.size(); ++i) {
        subgenres.push_back(substrings[i]);
    }
    creation.setSubgenres(subgenres);

    return creation;
}

string SerializerDeserializer::serializeBook(Book book) {
    string serializedBook = serializeCreation(book.getCreation());
    serializedBook += '#'; // We mark with a # the ending of a creation

    serializedBook += Utility::getStringForNumber(book.getISBN().size());
    serializedBook += ',';
    serializedBook += book.getISBN();

    serializedBook += Utility::getStringForNumber(book.getPublisher().size());
    serializedBook += ',';
    serializedBook += book.getPublisher();

    serializedBook += Utility::getStringForNumber((int)book.getPublicationYear());
    serializedBook += ',';

    serializedBook += Utility::getStringForFloat(book.getRating());

    return serializedBook;
}

Book SerializerDeserializer::deserializeBook(string serializedBook) {
    Book book;
    vector<string> substrings;
    string current;
    substrings = Utility::splitString(serializedBook, '#', 2);

    //First part is the serialized creation.
    book.setCreation(deserializeCreation(substrings[0]));
    current = substrings[1];
    // Split by comma, to get the length of the ISBN
    substrings = Utility::splitString(current, ',', 2);
    int substringLength = atoi(substrings[0].c_str());
    book.setISBN(substrings[1].substr(0, substringLength));
    current = substrings[1].substr(substringLength, string::npos);

    // Split by comma, to get the length of publisher
    substrings = Utility::splitString(current, ',', 2);
    substringLength = atoi(substrings[0].c_str());
    book.setPublisher(substrings[1].substr(0, substringLength));
    current = substrings[1].substr(substringLength, string::npos);

    // Get the publication year
    substrings = Utility::splitString(current, ',', 2);
    book.setPublicationYear((unsigned int)atoi(substrings[0].c_str()));
    current = substrings[1];

    // Get the rating
    book.setRating((float)atof(current.c_str()));

    return book;
}

string SerializerDeserializer::serializeBookList(list<Book> books) {
    string result;
    result += Utility::getStringForNumber(books.size());
    result += ",";
    for(list<Book>::iterator it = books.begin(); it != books.end(); ++it) {
        result += serializeBook(*it);
        result += "`";
    }
    return result;
}

list<Book> SerializerDeserializer::deserializeBookList(string serializedBooks) {
    list<Book> result;
    string current;
    vector<string> substrings = Utility::splitString(serializedBooks, ',', 2);
    int listSize = atoi(substrings[0].c_str());
    current = substrings[1];
    substrings = Utility::splitString(current, '`', listSize);
    for(int i = 0; i < listSize; ++i) {
        result.push_back(deserializeBook(substrings[i]));
    }
    return result;
}

string SerializerDeserializer::serializeUser(User user) {
    string serializedUser = user.getUsername();
    serializedUser += "#";
    serializedUser += user.getPassword();
    return serializedUser;
}

User SerializerDeserializer::deserializeUser(string serializedUser) {
    User user;
    vector<string> properties = Utility::splitString(serializedUser, '#', 2);
    user.setUsername(properties[0]);
    user.setPassword(properties[1]);
    return user;
}

string SerializerDeserializer::serializeRating(Rating rating) {
    string result;
    result += Utility::getStringForNumber(rating.getISBN().size());
    result += ",";
    result += rating.getISBN();
    result += Utility::getStringForNumber(rating.getRating());
    return result;
}

Rating SerializerDeserializer::deserializeRating(string serializedRating) {
    Rating rating;
    vector<string> substrings = Utility::splitString(serializedRating, ',', 2);
    int ISBNLength = atoi(substrings[0].c_str());
    rating.setISBN(substrings[1].substr(0, ISBNLength));
    rating.setRating(atoi(substrings[1].substr(ISBNLength, string::npos).c_str()));
    return rating;
}
