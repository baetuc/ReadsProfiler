/**
    Author: Ciprian Baetu
    Date: 01 Jan 2016
*/

#include <string>
#include <iostream>
#include <list>
#include <sqlite3.h>
#include "Book.h"
#include "Author.h"
#include "Creation.h"
#include "Utility.h"

#define DATABASE "Books.db"

using namespace std;

/* The query will be sent from client to server as a serialized Book,
 the server will deserialize the message and the newly created object
 will represent the "query" to the database.
*/
class DatabaseQueries {
private:
    static string getStatementForTitle(Creation creation, bool& addedSomething);
    static string getStatementForVolume(Creation creation, bool& addedSomething);
    static string getStatementForAuthors(Creation creation, bool& addedSomething);
    static string getStatementForGenres(Creation creation, bool& addedSomething);
    static string getStatementForSubgenres(Creation creation, bool& addedSomething);

    static string getStatementForISBN(Book query);
    static list<string> getISBNForBooksThatMatchTheQuery(Book query);
    static string getStringFromISBN(string ISBN, sqlite3* database, string field);
    static int getIntFromISBN(string ISBN, sqlite3* database, string field);
    static double getDoubleFromISBN(string ISBN, sqlite3* database, string field);
    static list<string> getListOfStringFromISBN(string ISBN, sqlite3* database, string field);
    static list<Author> getAuthorsFromISBN(string ISBN, sqlite3* database, string field);
    static Book getBookFromISBN(string ISBN);

public:
    static list<Book> getResponseToQuery(Book query);
};

string DatabaseQueries::getStatementForTitle(Creation creation, bool& addedSomething) {
    string statement;
    if(!creation.getTitle().empty()) {
        statement += " where title = '";
        statement += creation.getTitle();
        statement += "' ";
        addedSomething = true;
    }
    return statement;
}

// If volume is equal to 0, then we will set it to null (the returned string is empty)
string DatabaseQueries::getStatementForVolume(Creation creation, bool& addedSomething) {
    string statement;
    if(creation.getVolume()) {
        statement += addedSomething == true ? " and " : " where ";
        statement += " volume = ";
        statement += Utility::getStringForNumber((int) creation.getVolume());
        addedSomething = true;
    }
    return statement;
}

string DatabaseQueries::getStatementForAuthors(Creation creation, bool& addedSomething) {
    string statement;
    list<Author> authors = creation.getAuthors();
    if(authors.size()) {
        statement += addedSomething == true ? " and (" : " where (";
        addedSomething = true;
    }
    for(list<Author>::iterator it = authors.begin(); it != authors.end(); ++it) {
        if(it != authors.begin()) {
            statement += " or ";
        }
        statement += " ( ";
        bool explicitFirstName = false;
        if (!it->getFirstName().empty()) {
            statement += "firstName = '";
            statement += it->getFirstName();
            statement += "' ";
            explicitFirstName = true;
        }
        if (!it->getSecondName().empty()) {
            if(explicitFirstName) {
                statement += " and ";
            }
            statement += "secondName = '";
            statement += it->getSecondName();
            statement += "' ";
        }
        statement += " ) ";
    }
    if(authors.size()) {
        statement += ") ";
    }
    return statement;
}

string DatabaseQueries::getStatementForGenres(Creation creation, bool& addedSomething) {
    string statement;
    list<string> genres = creation.getGenres();
    if(genres.size()) {
        statement += addedSomething == true ? " and genre in (" : " where genre in ( ";
        addedSomething = true;
    }
    for(list<string>::iterator it = genres.begin(); it != genres.end(); ++it) {
        if(it != genres.begin()) {
            statement += " , ";
        }
        statement += " '";
        statement += *it;
        statement += "' ";
    }
    if(genres.size()) {
        statement += " ) ";
    }
    return statement;
}

string DatabaseQueries::getStatementForSubgenres(Creation creation, bool& addedSomething) {
    string statement;
    list<string> subgenres = creation.getSubgenres();
    if(subgenres.size()) {
        statement += addedSomething == true ? " and subgenre in (" : " where subgenre in ( ";
        addedSomething = true;
    }
    for(list<string>::iterator it = subgenres.begin(); it != subgenres.end(); ++it) {
        if(it != subgenres.begin()) {
            statement += " , ";
        }
        statement += " '";
        statement += *it;
        statement += "' ";
    }
    if(subgenres.size()) {
        statement += " ) ";
    }
    return statement;
}

/* First we will select the ISBNs of the books that match the
    query. */
string DatabaseQueries::getStatementForISBN(Book query) {
    string statement = "select distinct ISBN from previewInformation ";
    bool addedSomething = false;
    Creation creation = query.getCreation();

    statement += getStatementForTitle(creation, addedSomething);
    statement += getStatementForVolume(creation, addedSomething);
    statement += getStatementForAuthors(creation, addedSomething);
    statement += getStatementForGenres(creation, addedSomething);
    statement += getStatementForSubgenres(creation, addedSomething);
    statement += ";";
    return statement;
}

// Returns the ISBN of all the books that match the query
list<string> DatabaseQueries::getISBNForBooksThatMatchTheQuery(Book query, sqlite3* database) {
    char* statement = 0;
    int response;

    /* Create SQL statement */
    statement = getISBNForQuery(query).c_str();

    list<string> ISBNs;
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* ISBN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        ISBNs.push_back(string(ISBN));
    }
    sqlite3_finalize(stmt);
    return ISBNs;
}

string DatabaseQueries::getStringFromISBN(string ISBN, sqlite3* database, string field) {
    char* statement = "select distinct " << field << " from previewInformation where ISBN='" << ISBN << "';";
    int response;
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return string(result);
}

int DatabaseQueries::getIntFromISBN(string ISBN, sqlite3* database, string field) {
    char* statement = "select distinct " << field << " from previewInformation where ISBN='" << ISBN << "';";
    int response;
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int result = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return result;
}

list<Author> DatabaseQueries::getAuthorsFromISBN(string ISBN, sqlite3* database) {
    list<Author> authors;
    Author author;

    char* statement = "select firstName, secondName from previewInformation where ISBN='" << ISBN << "';";
    int response;
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* secondName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        author.setFirstName(string(firstName));
        author.setSecondName(string(secondName));
        authors.push_back(author);
    }
    sqlite3_finalize(stmt);
    return authors;
}

string DatabaseQueries::getListOfStringFromISBN(string ISBN, sqlite3* database, string field) {
    char* statement = "select distinct " << field << " from previewInformation where ISBN='" << ISBN << "';";
    int response;
    sqlite3_stmt *stmt;
    list<string> result;
    result = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* line = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        result.push_back(string(line));
    }
    sqlite3_finalize(stmt);
    return result;
}

double DatabaseQueries::getDoubleFromISBN(string ISBN, sqlite3* database, string field) {
    char* statement = "select distinct " << field << " from previewInformation where ISBN='" << ISBN << "';";
    int response;
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (result != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        double result = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return result;
}

Book DatabaseQueries::getBookFromISBN(string ISBN, sqlite3* database) {
    Book book;
    Creation creation;
    creation.setTitle(getStringFromISBN(ISBN, database, "title"));
    creation.setVolume((unsigned int) getIntFromISBN(ISBN, database, "volume"));
    creation.setAuthors(getAuthorsFromISBN(ISBN, database));
    creation.setGenres(getListOfStringFromISBN(ISBN, database, "genre"));
    creation.setSubgenres(getListOfStringFromISBN(ISBN, database, "subgenre"));

    book.setCreation(creation);

    book.setISBN(ISBN);
    book.setPublisher(getStringFromISBN(ISBN, database, "publisher"));
    book.setPublicationYear((unsigned int) getIntFromISBN(ISBN, database, "publicationYear"));
    book.setRating((float) getDoubleFromISBN(ISBN, database, "rating"));

    return book;
}

list<Book> DatabaseQueries::getResponseToQuery(Book query) {
    sqlite3* database;
    char* statement = 0;
    int response;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        // TODO : send error message to child?
        cerr << "Can't open database: " << sqlite3_errmsg(database) << '\n';
        exit(0);
    }

    list<string> ISBNs = getISBNForBooksThatMatchTheQuery(query, database);
    list<Book> result;
    for(list<string>::iterator it = ISBNs.begin(); it != ISBNs.end(); ++it) {
        result.push_back(getBookFromISBN(*it));
    }

    sqlite3_close(database);

    return result;
}
