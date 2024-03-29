/**
    Author: Ciprian Baetu
    Date: 01 Jan 2016
*/

#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include "sqlite3.h"
#include <stdlib.h>
#include "Book.h"
#include "Author.h"
#include "Creation.h"
#include "Utility.h"

#define DATABASE "Books.db"
#define USERS "Users.db"

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
    static string getStatementForISBN(Book query);
    static list<string> getISBNForBooksThatMatchTheQuery(Book query, sqlite3* database);
    static string getStringFromISBN(string ISBN, sqlite3* database, string field);
    static int getIntFromISBN(string ISBN, sqlite3* database, string field);
    static double getDoubleFromISBN(string ISBN, sqlite3* database, string field);
    static list<string> getListOfStringFromISBN(string ISBN, sqlite3* database, string field);
    static list<Author> getAuthorsFromISBN(string ISBN, sqlite3* database);
    static Book getBookFromISBN(string ISBN, sqlite3* database);

public:
    static list<Book> getResponseToQuery(Book query);
    static void rateBook(int rate, string ISBN); // method that updates book rating
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
    statement += ";";
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
        cerr << "SELECT failed BOOKS THAT MATCH: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
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
        cerr << "SELECT failed STRING FROM: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
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
        cerr << "SELECT failed INT: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
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
    string statementString = "select firstName, secondName from previewInformation where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT failed AUTHORS: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
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
        cerr << "SELECT failed LIST STRING: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        char* line = (char*)(sqlite3_column_text(stmt, 0));
        string resultt(line);
        result.push_back(string(line));
    }
    sqlite3_finalize(stmt);
    return result;
}

double DatabaseQueries::getDoubleFromISBN(string ISBN, sqlite3* database, string field) {
    const char* statement;
    string statementString = "select distinct ";
    statementString += field;
    statementString += " from previewInformation where ISBN='";
    statementString += ISBN;
    statementString += "';";
    statement = statementString.c_str();
    int response;
    sqlite3_stmt *stmt;
    double result;
    response = sqlite3_prepare_v2(database, statement, -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT failed FLOAT: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
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
    book.setRating((float) getDoubleFromISBN(ISBN, database, "rating"));

    return book;
}

list<Book> DatabaseQueries::getResponseToQuery(Book query) {
    sqlite3* database;
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
        result.push_back(getBookFromISBN(*it, database));
    }

    sqlite3_close(database);

    return result;
}

void DatabaseQueries::rateBook(int rate, string ISBN) {
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        // TODO : send error message to child?
        cerr << "Can't open database: " << sqlite3_errmsg(database) << '\n';
        exit(0);
    }
    int numberOfRates;
    double oldRate;
    string statement = "select numberOfRates from books where ISBN='";
    statement += ISBN;
    statement += "';";

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        numberOfRates = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    sqlite3_stmt *stmt2;
    statement = "select rating from books where ISBN='";
    statement += ISBN;
    statement += "';";
    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt2, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    if (sqlite3_step(stmt2) == SQLITE_ROW) {
        oldRate = sqlite3_column_double(stmt2, 0);
    }
    sqlite3_finalize(stmt2);

    double newRating = (oldRate * (double) numberOfRates + (double)rate)/ (double)(numberOfRates + 1);
    statement = "update books set numberOfRates=numberOfRates+1, rating=";
    statement += Utility::getStringForFloat((float) newRating);
    statement += " where ISBN='";
    statement += ISBN;
    statement += "';";

    sqlite3_stmt *stmt3;
    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt3, NULL);
    if (response != SQLITE_OK) {
        cerr << "UPDATE failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    if (sqlite3_step(stmt3) != SQLITE_DONE) {
        cerr << "UPDATE failed: " << sqlite3_errmsg(database) << '\n';
    }
    sqlite3_finalize(stmt3);
    sqlite3_close(database);
}


string DatabaseQueries::getPath(string ISBN) {
    sqlite3* database;
    int response;
    sqlite3_stmt *stmt;

    /* Open database */
    response = sqlite3_open(DATABASE, &database);
    if(response){
        // TODO : send error message to child?
        cerr << "Can't open database: " << sqlite3_errmsg(database) << '\n';
        exit(0);
    }

    string statement = "select path from books where ISBN='";
    statement += ISBN;
    statement += "';";

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT path failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
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
    response = sqlite3_open(USERS, &database);
    if(response){
        // TODO : send error message to child?
        cerr << "Can't open database: " << sqlite3_errmsg(database) << '\n';
        exit(0);
    }

    string statement = "select password from userPassword where userName='";
    statement += userName;
    statement += "';";

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT user failed: " << sqlite3_errmsg(database) << '\n';
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
    response = sqlite3_open(USERS, &database);
    if(response){
        // TODO : send error message to child?
        cerr << "Can't open database: " << sqlite3_errmsg(database) << '\n';
        exit(0);
    }

    string statement = "select count(*) from userPassword where userName='";
    statement += userName;
    statement += "';";
    bool existsUser = true;

    response = sqlite3_prepare_v2(database, statement.c_str(), -1, &stmt, NULL);
    if (response != SQLITE_OK) {
        cerr << "SELECT count(*) failed: " << sqlite3_errmsg(database) << '\n';
        exit(0); // TODO: think about throw?
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {

        int haha = sqlite3_column_int(stmt, 0);
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
            cerr << "INSERT failed: " << sqlite3_errmsg(database) << '\n';
            exit(0); // TODO: think about throw?
        }
        if (sqlite3_step(stmt2) != SQLITE_DONE) {
            cerr << "INSERT failed: " << sqlite3_errmsg(database) << '\n';
        }
        sqlite3_finalize(stmt2);
    }
    sqlite3_close(database);
    return existsUser;
}
