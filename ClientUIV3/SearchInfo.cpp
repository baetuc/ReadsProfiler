#include "SearchInfo.h"

using namespace std;

string SearchInfo::getUsername() {return username;}
Book SearchInfo::getQuery() {return query;}
void SearchInfo::setUsername(string username) {this->username = username;}
void SearchInfo::setQuery(Book query) {this->query = query;}
