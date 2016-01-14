#include <iostream>
#include <string>
#include <list>
#include <map>
#include <set>
#include "DatabaseQueries.h"

using namespace std;

class Statistic {
    string username;
    map<string, int> genreRank;
    map<string, int> subgenreRank;
    map<int, int> authorRank;
    map<string, int> bookRates;
    map<string, int> getRankStringStatistic(map<string, int> statistic);
    map<int, int> getRankIntStatistic(map<int, int> statistic);

public:
    Statistic();
    Statistic(string username);
    string getUsername() {return username;}
    map<string, int> getGenreRank() {return genreRank;}
    map<string, int> getSubgenreRank() {return subgenreRank;}
    map<int, int> getAuthorRank() {return authorRank;}
    map<string, int> getBookRates() {return bookRates;}
};

// Give a rank to each author in order to apply Spearman distance
map<int, int> Statistic::getRankIntStatistic(map<int, int> statistic) {
    map<int, int> viewsStatistic;
    // Firstly we introduce every value in a map, then we calculate the rank for that value
    for(map<int, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        ++viewsStatistic[it->second];
    }

    int rank = 1;
    for(map<int, int>::reverse_iterator it = viewsStatistic.rbegin(); it != viewsStatistic.rend(); ++it) {
        it->second = rank++;
    }

    // For each author in the initial statistic, we assign a rank based on the searches
    map<int, int> result;
    for(map<int, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        result[it->first] = viewsStatistic[it->second];
    }

    return result;
}

map<string, int> Statistic::getRankStringStatistic(map<string, int> statistic) {
    map<int, int> viewsStatistic;
    // Firstly we introduce every value in a map, then we calculate the rank for that value
    for(map<string, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        ++viewsStatistic[it->second];
    }

    int rank = 1;
    for(map<int, int>::reverse_iterator it = viewsStatistic.rbegin(); it != viewsStatistic.rend(); ++it) {
        it->second = rank++;
    }
    map<string, int> result;
    for(map<string, int>::iterator it = statistic.begin(); it != statistic.end(); ++it) {
        result[it->first] = viewsStatistic[it->second];
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
    genreRank = getRankStringStatistic(genreStatistic);
    subgenreRank = getRankStringStatistic(subgenreStatistic);
    authorRank = getRankIntStatistic(authorStatistic);
    bookRates = DatabaseQueries::getBookRatesForUser(username);

}
