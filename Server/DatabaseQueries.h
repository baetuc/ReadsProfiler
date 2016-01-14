/**
    Author: Ciprian Baetu
    Date: 01 Jan 2016
*/

#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include "sqlite3.h"
#include <stdlib.h>
#include "Book.h"
#include "Author.h"
#include "Creation.h"
#include "Utility.h"
#include "SearchInfo.h"

#define DATABASE "Books.db"
#pragma once

using namespace std;

/* The query will be sent from client to server as a serialized Book,
 the server will deserialize the message and the newly created object
 will represent the "query" to the database.
*/
class DatabaseQueries {
private:
    // these methods are to query the database and give result to a search
    static string getStatementForTitle(Creation creation, bool& addedSomething);
    static string getStatementForVolume(Creation creation, bool& addedSomething);
    static string getStatementForAuthors(Creation creation, bool& addedSomething);
    static string getStatementForGenres(Creation creation, bool& addedSomething);
    static string getStatementForSubgenres(Creation creation, bool& addedSomething);
    static string getStatementForPublisher(Book book, bool& addedSomething);
    static string getStatementForISBNQuery(Book book, bool& addedSomething);
    static string getStatementForYear(Book book, bool& addedSomething);
    static string getStatementForRating(Book book, bool& addedSomething);
    static string getStatementForISBN(Book query);
    static list<string> getISBNForBooksThatMatchTheQuery(Book query, sqlite3* database);
    static string getStringFromISBN(string ISBN, sqlite3* database, string field);
    static int getIntFromISBN(string ISBN, sqlite3* database, string field);
    static double getRatingFromISBN(string ISBN, sqlite3* database);
    static list<string> getListOfStringFromISBN(string ISBN, sqlite3* database, string field);
    static list<Author> getAuthorsFromISBN(string ISBN, sqlite3* database);
    static Book getBookFromISBN(string ISBN, sqlite3* database);

    static void saveGenres(string username, list<string> genres, sqlite3* database);
    static void saveSubgenres(string username, list<string> subgenres, sqlite3* database);
    static void saveAuthors(string username, list<Author> authors, sqlite3* database);


public:
    static list<Book> getResponseToQuery(Book query);
    static void saveQuery(SearchInfo search);
    static list<string> getTopBooks(int top);
    static list<Book> getBooksFromISBNList(list<string> ISBN);
    static map<string, int> getStringStatisticForUser(string username, string field);
    static map<int, int> getIntStatisticForUser(string username, string field);
    static map<string, int> getBookRatesForUser(string username);
    static list<string> getOtherActiveUsers(string username);
    static string rateBook(int rate, string ISBN, string username); // method that updates book rating
    static string getPath(string ISBN); // method that returns book's path
    static bool getPasswordForUser(string user, string& password);
    static bool createUser(string user, string password);

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

string DatabaseQueries::getStatementForPublisher(Book book, bool& addedSomething) {
    string statement;
    if(!book.getPublisher().empty()) {
        statement += addedSomething == true ? " and " : " where ";
        statement += " publisher = '";
        statement += book.getPublisher();
        statement += "' ";
        addedSomething = true;
    }
    return statement;
}

string DatabaseQueries::getStatementForISBNQuery(Book book, bool& addedSomething) {
    string statement;
    if(!book.getISBN().empty()) {
        statement += addedSomething == true ? " and " : " where ";
        statement += " ISBN = '";
        statement += book.getPublisher();
        statement += "' ";
        addedSomething = true;
    }
    return statement;
}

string DatabaseQueries::getStatementForYear(Book book, bool& addedSomething) {
    string statement;
    if(book.getPublicationYear()) {
        statement += addedSomething == true ? " and " : " where ";
        statement += " publicationYear = ";
        statement += Utility::getStringForNumber((int) book.getPublicationYear());
        addedSomething = true;
    }
    return statement;
}
string DatabaseQueries::getStatementForRating(Book book, bool& addedSomething) {
    string statement;
    if(book.getRating()) {
        statement += addedSomething == true ? " and " : " where ";
        statement += " rating >= ";
        statement += Utility::getStringForFloat(book.getRating());
        addedSomething = true;
    }
    return statement;
}

/* First we will select the ISBNs of the books that match the
    query. */
string DatabaseQueries::getStatementForISBN(Book query) {
    string statement = "select distinct isbn from previewInformation ";
    bool addedSomething = false;
    Creation creation = query.getCreation();

    statement += getStatementForTitle(creation, addedSomething);
    statement += getStatementForVolume(creation, addedSomething);
    statement += getStatementForAuthors(creation, addedSomething);
    statement += getStatementForGenres(creation, addedSomething);
    statement += getStatementForSubgenres(creation, addedSomething);
    statement += getStatementForPublisher(query, addedSomething);
    statement += getStatementForISBNQuery(query, addedSomething);
    statement += getStatementForYear(query, addedSomething);
    statement += getStatementForRating(query, addedSomething);
    statement += ";";
    cout << "Statement for ISBN: " << statement << '\n';
    return statement;
}

// Returns the ISBN of all the books that match the query
list<string> DatabaseQueries::getISBNForBooksThatMatchTheQuery(Book query, sqlite3* database) {
    int response;
    /* Create SQL statement */
    string ISBNStatement = getStatementForISBN(query);
    char* statement = new char[ISBNStatement.length() + 1];
    strcpy(statement, ISBNStatement.c_str());
    //statement = "select distinct isbn from previewInformation  where  volume = 1;";

    list<string> ISBNs;
    sqlite3_stmt *stmt;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT failed BOOKS THAT MATCH ", sqlite3_errmsg(database), "\n");
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char* ISBN = (char*)(sqlite3_column_text(stmt, 0));
        ISBNs.push_back(string(ISBN));
    }
    sqlite3_finalize(stmt);
    return ISBNs;

}

string DatabaseQueries::getStringFromISBN(string ISBN, sqlite3* database, string field) {
    const char* statement;
    string statementString = "select distinct ";
    statementString += field;
    statementString += " from previewInformation where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT failed STRING FROM: ", sqlite3_errmsg(database), "\n");
    }
    string resultInString;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        char* result = (char*)(sqlite3_column_text(stmt, 0));
        int length = sqlite3_column_bytes(stmt, 0);
        resultInString.assign(result, length);
    }
    sqlite3_finalize(stmt);
    return resultInString;
}

int DatabaseQueries::getIntFromISBN(string ISBN, sqlite3* database, string field) {
    const char* statement;
    string statementString = "select distinct ";
    statementString += field;
    statementString += " from previewInformation where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT failed INT: ", sqlite3_errmsg(database), "\n");
    }
    int result;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return result;
}

list<Author> DatabaseQueries::getAuthorsFromISBN(string ISBN, sqlite3* database) {
    list<Author> authors;
    Author author;

    const char* statement;
    string statementString = "select distinct firstName, secondName from previewInformation where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT failed AUTHORS: ", sqlite3_errmsg(database), "\n");
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char* firstName = (char*)(sqlite3_column_text(stmt, 0));
        char* secondName = (char*)(sqlite3_column_text(stmt, 1));
        author.setFirstName(string(firstName));
        author.setSecondName(string(secondName));
        authors.push_back(author);
    }
    sqlite3_finalize(stmt);
    return authors;
}

list<string> DatabaseQueries::getListOfStringFromISBN(string ISBN, sqlite3* database, string field) {
    const char* statement;
    string statementString = "select distinct ";
    statementString += field;
    statementString += " from previewInformation where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    list<string> result;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT failed LIST STRING: ", sqlite3_errmsg(database), "\n");
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char* line = (char*)(sqlite3_column_text(stmt, 0));
        string resultt(line);
        result.push_back(string(line));
    }
    sqlite3_finalize(stmt);
    return result;
}

double DatabaseQueries::getRatingFromISBN(string ISBN, sqlite3* database) {
    const char* statement;
    string statementString = "select avg(rating) from userRating where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    double result;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT rating failed: ", sqlite3_errmsg(database), "\n");
        exit(0);
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_double(stmt, 0);
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
    book.setRating(getRatingFromISBN(ISBN, database));

    return book;
}

list<Book> DatabaseQueries::getBooksFromISBNList(list<string> ISBN) {
    sqlite3* database;
    int response;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    list<Book>result;
    for(list<string>::iterator it = ISBN.begin(); it != ISBN.end(); ++it) {
        result.push_back(getBookFromISBN(*it, database));
        cout << SerializerDeserializer::serializeBook(getBookFromISBN(*it, database)) << '\n';
    }
    sqlite3_close(database);
    return result;
}

list<Book> DatabaseQueries::getResponseToQuery(Book query) {
    sqlite3* database;
    int response;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    list<string> ISBNs = getISBNForBooksThatMatchTheQuery(query, database);
    list<Book> result;
    for(list<string>::iterator it = ISBNs.begin(); it != ISBNs.end(); ++it) {
        result.push_back(getBookFromISBN(*it, database));
    }

    sqlite3_close(database);

    return result;
}

void DatabaseQueries::saveGenres(string username, list<string> genres, sqlite3* database) {
    string genreStatement = "insert into userPreferences values('";
    genreStatement += username;
    genreStatement += "','";
    string finalGenreStatement = "', null, null);";
    for(list<string>::iterator it = genres.begin(); it != genres.end(); ++it) {
        string statementString = genreStatement;
        statementString += *it;
        statementString += finalGenreStatement;
        const char* statement = statementString.c_str();
        sqlite3_stmt *stmt;
        int response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
        if (response != SQLITE_OK) {
            throw Utility::concatenateStrings("INSERT genre failed: ", sqlite3_errmsg(database), "\n");
            exit(0);
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw Utility::concatenateStrings("INSERT genre failed: ", sqlite3_errmsg(database), "\n");
        }
        sqlite3_finalize(stmt);
    }
}

void DatabaseQueries::saveSubgenres(string username, list<string> subgenres, sqlite3* database) {
    string subgenreStatement = "insert into userPreferences values('";
    subgenreStatement += username;
    subgenreStatement += "',null,'";
    string finalSubgenreStatement = "', null);";
    for(list<string>::iterator it = subgenres.begin(); it != subgenres.end(); ++it) {
        string statementString = subgenreStatement;
        statementString += *it;
        statementString += finalSubgenreStatement;
        const char* statement = statementString.c_str();
        sqlite3_stmt *stmt;
        int response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
        if (response != SQLITE_OK) {
            throw Utility::concatenateStrings("INSERT subgenre failed: ", sqlite3_errmsg(database), "\n");
            exit(0);
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw Utility::concatenateStrings("INSERT subgenre failed: ", sqlite3_errmsg(database), "\n");
        }
        sqlite3_finalize(stmt);
    }
}

void DatabaseQueries::saveAuthors(string username, list<Author> authors, sqlite3* database) {
    string authorStatement = "insert into userPreferences values('";
    authorStatement += username;
    authorStatement += "',null, null,";
    string finalAuthorStatement = ");";
    for(list<Author>::iterator it = authors.begin(); it != authors.end(); ++it) {
        // find the autID for those authors;
        string autIDStatement = "select autID from authors where firstName='";
        autIDStatement += it->getFirstName();
        autIDStatement += "' and secondName = '";
        autIDStatement += it->getSecondName();
        autIDStatement += "';";

        sqlite3_stmt *stmt;
        int response = sqlite3_prepare_v2(database, autIDStatement.c_str(), -1, &stmt, NULL);
        if (response != SQLITE_OK) {
            throw Utility::concatenateStrings("SELECT autID failed: ", sqlite3_errmsg(database), "\n");
            exit(0);
        }
        int autID = 0; // default for not found
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            autID = sqlite3_column_int(stmt, 0);
            cout << "Author ID is: " << autID << '\n';
        }
        sqlite3_finalize(stmt);

        sqlite3_stmt* stmt2;

        string statementString = authorStatement;
        statementString += Utility::getStringForNumber(autID);
        statementString += finalAuthorStatement;
        const char* statement = statementString.c_str();

        response = sqlite3_prepare_v2(database, statement, -1, &stmt2, NULL);
        if (response != SQLITE_OK) {
            throw Utility::concatenateStrings("INSERT autID failed: ", sqlite3_errmsg(database), "\n");
            exit(0);
        }

        if (sqlite3_step(stmt2) != SQLITE_DONE) {

            throw Utility::concatenateStrings("INSERT autID failed: ", sqlite3_errmsg(database), "\n");
        }
        sqlite3_finalize(stmt2);
    }
}

void DatabaseQueries::saveQuery(SearchInfo search) {
    sqlite3* database;

    /* Open database */
    int response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    string username = search.getUsername();
    Book book = search.getQuery();
    saveGenres(username, book.getCreation().getGenres(), database);
    saveSubgenres(username, book.getCreation().getSubgenres(), database);
    saveAuthors(username, book.getCreation().getAuthors(), database);

    sqlite3_close(database);
}

// Method that returns previous rating from a user on a specific book, if he rated
// that book, otherwise 0
string DatabaseQueries::rateBook(int rate, string ISBN, string username) {
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }
    int rating = 0;
    string statement = "select rating from userRating where ISBN='";
    statement += ISBN;
    statement += "' and username='";
    statement += username;
    statement += "';";

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT rate count failed: ", sqlite3_errmsg(database), "\n");
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        rating = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (rating != 0) {
        // send to the user the message that he cannot rate anymore that book
        return Utility::getStringForNumber(rating);
    }

    // else, the user didn't rate that book before
    sqlite3_stmt *stmt2;
    statement = "insert into userRating values('";
    statement += username;
    statement += "','";
    statement += ISBN;
    statement += "',";
    statement += Utility::getStringForNumber(rate);
    statement += ");";
    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt2, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("INSERT rate failed: ", sqlite3_errmsg(database), "\n");
    }
    if (sqlite3_step(stmt2) != SQLITE_DONE) {
        throw Utility::concatenateStrings("INSERT rate failed: ", sqlite3_errmsg(database), "\n");
    }
    sqlite3_finalize(stmt2);

    sqlite3_close(database);
    return "0";
}

// ------------------------------------ RECOMMEND -----------------------------------------

list<string> DatabaseQueries::getTopBooks(int top) {
    list<string> result;

    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }
    int rating = 0;
    string auxStatement = "select isbn from previewInformation order by rating desc limit ";
    auxStatement += Utility::getStringForNumber(top);
    auxStatement += ";";

    const char* statement = auxStatement.c_str();

    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT top books failed: ", sqlite3_errmsg(database), "\n");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int length = sqlite3_column_bytes(stmt, 1);
        if(length > 0) {
            char* isbn = (char*)(sqlite3_column_text(stmt, 1));
            cout << "Found adding isbn: " << isbn << '\n';
            result.push_back(string(isbn));
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return result;
}

map<string, int> DatabaseQueries::getStringStatisticForUser(string username, string field) {
    map<string, int> statistic;

    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }
    int rating = 0;
    string auxStatement = "select count(*),";
    auxStatement += field;
    auxStatement += " from userPreferences where username='";
    auxStatement += username;
    auxStatement += "' group by ";
    auxStatement += field;
    auxStatement += ";";
    const char* statement = auxStatement.c_str();

    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT string statistic failed: ", sqlite3_errmsg(database), "\n");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);

        int length = sqlite3_column_bytes(stmt, 1);
        if(length > 0) {
            char* result = (char*)(sqlite3_column_text(stmt, 1));
            statistic[string(result)] = count;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return statistic;
}

map<int, int> DatabaseQueries::getIntStatisticForUser(string username, string field) {
    map<int, int> statistic;

    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }
    int rating = 0;
    string auxStatement = "select count(*),";
    auxStatement += field;
    auxStatement += " from userPreferences where username='";
    auxStatement += username;
    auxStatement += "' group by ";
    auxStatement += field;
    auxStatement += ";";
    const char* statement = auxStatement.c_str();

    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT int statistic failed: ", sqlite3_errmsg(database), "\n");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);

        int autID = sqlite3_column_int(stmt, 1);
        statistic[autID] = count;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return statistic;
}

map<string, int> DatabaseQueries::getBookRatesForUser(string username) {
    map<string, int> statistic;

    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }
    string auxStatement = "select rating, isbn from userRating where username='";
    auxStatement += username;
    auxStatement += "';";
    const char* statement = auxStatement.c_str();

    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT book rates failed: ", sqlite3_errmsg(database), "\n");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int rating = sqlite3_column_int(stmt, 0);

        int length = sqlite3_column_bytes(stmt, 1);
        if(length > 0) {
            char* isbn = (char*)(sqlite3_column_text(stmt, 1));
            statistic[string(isbn)] = rating;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return statistic;
}

list<string> DatabaseQueries::getOtherActiveUsers(string username) {
    list<string> users;
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    string statement = "select distinct username from userRating where username != '";
    statement += username;
    statement += "';";

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT usernames failed: ", sqlite3_errmsg(database), "\n");
    }
    string resultInString;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int length = sqlite3_column_bytes(stmt, 0);
        if(length > 0) {
            char* result = (char*)(sqlite3_column_text(stmt, 0));
            resultInString.assign(result, length);
            users.push_back(resultInString);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return users;
}

// --------------------------------- END OF RECOMMEND -------------------------------------


string DatabaseQueries::getPath(string ISBN) {
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    string statement = "select path from books where ISBN='";
    statement += ISBN;
    statement += "';";
    //cout << "Path query: " << statement << '\n';

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT path failed: ", sqlite3_errmsg(database), "\n");
    }
    string resultInString;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        char* result = (char*)(sqlite3_column_text(stmt, 0));
        int length = sqlite3_column_bytes(stmt, 0);
        resultInString.assign(result, length);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return resultInString;
}

// returns false if user doesn't exist
bool DatabaseQueries::getPasswordForUser(string userName, string& password) {
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    string statement = "select password from userPassword where userName='";
    statement += userName;
    statement += "';";

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT user failed: ", sqlite3_errmsg(database), "\n");
        exit(0); // TODO: think about throw?
    }
    bool existsUser = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existsUser = true;
        char* result = (char*)(sqlite3_column_text(stmt, 0));
        int length = sqlite3_column_bytes(stmt, 0);
        password.assign(result, length);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return existsUser;
}

// returns true if user existed already and false in the other case
// if returned false, it follows that the user created successfully
bool DatabaseQueries::createUser(string userName, string password) {
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt, *stmt2;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        throw Utility::concatenateStrings("Can't open database: ", sqlite3_errmsg(database), "\n");
    }

    string statement = "select count(*) from userPassword where userName='";
    statement += userName;
    statement += "';";
    bool existsUser = true;

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        throw Utility::concatenateStrings("SELECT COUNT(*) failed: ", sqlite3_errmsg(database), "\n");
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        existsUser = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if(!existsUser) {
        statement = "insert into userPassword values ('";
        statement += userName;
        statement += "','";
        statement += password;
        statement += "');";
        response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt2, NULL);
        if (response != SQLITE_OK) {
            throw Utility::concatenateStrings("INSERT user failed: ", sqlite3_errmsg(database), "\n");
        }
        if (sqlite3_step(stmt2) != SQLITE_DONE) {
            throw Utility::concatenateStrings("INSERT user failed: ", sqlite3_errmsg(database), "\n");
        }
        sqlite3_finalize(stmt2);
    }
    sqlite3_close(database);
    return existsUser;
}
