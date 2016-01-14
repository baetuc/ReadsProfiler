#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>
#include "DatabaseQueries.h"

using namespace std;

class Statistic {
    string username;
    map<string, int> genreTop;
    map<string, int> subgenreTop;
    map<int, int> authorTop;
    map<string, int> bookRates;
    map<string, int> getTopStringStatistic(int top, map<string, int> statistic);
    map<int, int> getTopIntStatistic(int top, map<int, int> statistic);

public:
    Statistic();
    Statistic(string username);
    string getUsername() {return username;}
    map<string, int> getGenreTop() {return genreTop;}
    map<string, int> getSubgenreTop() {return subgenreTop;}
    map<int, int> getAuthorTop() {return authorTop;}
    map<string, int> getBookRates() {return bookRates;}
};

map<int, int> Statistic::getTopIntStatistic(int top, map<int, int> statistic) {
    map<int, int> viewsStatistic;
    // firstly we count every value in the statistic
    for(map<int, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        ++viewsStatistic[it->second];
    }

    map<int, int> topMap;
    for(map<int, int>::reverse_iterator it = viewsStatistic.rbegin(); it != viewsStatistic.rend() && top > 0; ++it) {
        topMap[it->first] = it->second > top ? top : it->second;
        top -= it->second;
    }

    // in topMap we have for each value how many times it can appear in the final
    map<int, int> result;
    for(map<int, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        if(topMap[it->second] > 0) {
            --topMap[it->second];
            result[it->first] = it->second;
        }
    }

    return result;
}

map<string, int> Statistic::getTopStringStatistic(int top, map<string, int> statistic) {
    map<int, int> viewsStatistic;
    // firstly we count every value in the statistic
    for(map<string, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        ++viewsStatistic[it->second];
    }

    map<int, int> topMap;
    for(map<int, int>::reverse_iterator it = viewsStatistic.rbegin(); it != viewsStatistic.rend() && top > 0; ++it) {
        topMap[it->first] = it->second > top ? top : it->second;
        top -= it->second;
    }
    // in topMap we have for each value how many times it can appear in the final

    map<string, int> result;
    for(map<string, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        if(topMap[it->second] > 0) {
            --topMap[it->second];
            result[it->first] = it->second;
        }
    }

    return result;
}

Statistic::Statistic() {

}

Statistic::Statistic(string username) {
    this->username = username;
    map<string, int> genreStatistic = DatabaseQueries::getStringStatisticForUser(username, "genre");
    map<string, int> subgenreStatistic = DatabaseQueries::getStringStatisticForUser(username, "subgenre");
    map<int, int> authorStatistic = DatabaseQueries::getIntStatisticForUser(username, "autID");
    genreTop = getTopStringStatistic(TOP, genreStatistic);
    subgenreTop = getTopStringStatistic(TOP, subgenreStatistic);
    authorTop = getTopIntStatistic(TOP, authorStatistic);

    bookRates = DatabaseQueries::getBookRatesForUser(username);

}
