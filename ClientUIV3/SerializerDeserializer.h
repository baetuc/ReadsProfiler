/**
    Author: Ciprian Baetu
    Date: 31 Dec 2015
*/

#include <string>
#include <list>
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
