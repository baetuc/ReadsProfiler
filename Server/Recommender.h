#include <cmath>
#include "Statistic.h"
#include "BookScore.h"
#include <list>
#include <algorithm>
#include <vector>

#define TOP_WANTED 10
#define SCORE_THRESHOLD 5

using namespace std;

class Recommender {
    string username;

    map<string, double> userDistances;
    set<string> unreadRatedBooks;

    Statistic primary;
    map<string, Statistic> otherStatistics;

    double computeGenreSpearmanDistance(Statistic primary, Statistic other);
    double computeSubgenreSpearmanDistance(Statistic primary, Statistic other);
    double computeAuthorSpearmanDistance(Statistic primary, Statistic other);
    double computeSpearmanDistance(Statistic primary, Statistic other);
    double computePearsonCorrelation(Statistic primary, Statistic other);

    double computeUserDistance(Statistic primary, Statistic secondary);
    double getAverageRate(Statistic stat);
public:
    Recommender(string username);
    list<string> recommendBooks();

};

double Recommender::computeGenreSpearmanDistance(Statistic primary, Statistic other) {
    // First we compute spearman distance for genre
    map<string, int> primaryGenreTop = primary.getGenreTop();
    map<string, int> otherGenreTop = other.getGenreTop();

    set<string> impliedGenres;
    for(map<string, int>::iterator it = primaryGenreTop.begin(); it != primaryGenreTop.end(); ++it) {
        impliedGenres.insert(it->first);
    }
    for(map<string, int>::iterator it = otherGenreTop.begin(); it != otherGenreTop.end(); ++it) {
        impliedGenres.insert(it->first);
    }

    if(impliedGenres.size() < 2) {
        return 0;
    }

    double squareSum = 0;
    for(map<string, int>::iterator it = primaryGenreTop.begin(); it != primaryGenreTop.end(); ++it) {
        squareSum += (it->second - otherGenreTop[it->first]) * (it->second - otherGenreTop[it->first]);
    }
    for(map<string, int>::iterator it = otherGenreTop.begin(); it != otherGenreTop.end(); ++it) {
        if(primaryGenreTop.find(it->first) == primaryGenreTop.end()) {
            squareSum += (it->second - primaryGenreTop[it->first]) * (it->second - primaryGenreTop[it->first]);
        }
    }

    return 1 - ((double)(6 * squareSum) / (impliedGenres.size() * impliedGenres.size() - 1));
}

double Recommender::computeSubgenreSpearmanDistance(Statistic primary, Statistic other) {
    // First we compute spearman distance for genre
    map<string, int> primarySubgenreTop = primary.getSubgenreTop();
    map<string, int> otherSubgenreTop = other.getSubgenreTop();

    set<string> impliedSubgenres;
    for(map<string, int>::iterator it = primarySubgenreTop.begin(); it != primarySubgenreTop.end(); ++it) {
        impliedSubgenres.insert(it->first);
    }
    for(map<string, int>::iterator it = otherSubgenreTop.begin(); it != otherSubgenreTop.end(); ++it) {
        impliedSubgenres.insert(it->first);
    }

    if(impliedSubgenres.size() < 2) {
        return 0;
    }

    double squareSum = 0;
    for(map<string, int>::iterator it = primarySubgenreTop.begin(); it != primarySubgenreTop.end(); ++it) {
        squareSum += (it->second - otherSubgenreTop[it->first]) * (it->second - otherSubgenreTop[it->first]);
    }
    for(map<string, int>::iterator it = otherSubgenreTop.begin(); it != otherSubgenreTop.end(); ++it) {
        if(primarySubgenreTop.find(it->first) == primarySubgenreTop.end()) {
            squareSum += (it->second - primarySubgenreTop[it->first]) * (it->second - primarySubgenreTop[it->first]);
        }
    }

    return 1 - ((double)(6 * squareSum) / (impliedSubgenres.size() * impliedSubgenres.size() - 1));
}

double Recommender::computeAuthorSpearmanDistance(Statistic primary, Statistic other) {
    // First we compute spearman distance for genre
    map<int, int> primaryAuthorTop = primary.getAuthorTop();
    map<int, int> otherAuthorTop = other.getAuthorTop();

    set<int> impliedAuthors;
    for(map<int, int>::iterator it = primaryAuthorTop.begin(); it != primaryAuthorTop.end(); ++it) {
        impliedAuthors.insert(it->first);
    }
    for(map<int, int>::iterator it = otherAuthorTop.begin(); it != otherAuthorTop.end(); ++it) {
        impliedAuthors.insert(it->first);
    }

    if(impliedAuthors.size() < 2) {
        return 0;
    }

    double squareSum = 0;
    for(map<int, int>::iterator it = primaryAuthorTop.begin(); it != primaryAuthorTop.end(); ++it) {
        squareSum += (it->second - otherAuthorTop[it->first]) * (it->second - otherAuthorTop[it->first]);
    }
    for(map<int, int>::iterator it = otherAuthorTop.begin(); it != otherAuthorTop.end(); ++it) {
        if(primaryAuthorTop.find(it->first) == primaryAuthorTop.end()) {
            squareSum += (it->second - primaryAuthorTop[it->first]) * (it->second - primaryAuthorTop[it->first]);
        }
    }

    return 1 - ((double)(6 * squareSum) / (impliedAuthors.size() * impliedAuthors.size() - 1));
}

double Recommender::computeSpearmanDistance(Statistic primary, Statistic other) {
    cout << primary.getUsername() << " " << other.getUsername() << '\n';
    cout << (computeGenreSpearmanDistance(primary, other) + computeSubgenreSpearmanDistance(primary, other) +
        computeAuthorSpearmanDistance(primary, other)) / 3 << '\n';
    return (computeGenreSpearmanDistance(primary, other) + computeSubgenreSpearmanDistance(primary, other) +
        computeAuthorSpearmanDistance(primary, other)) / 3;
}


double Recommender::computePearsonCorrelation(Statistic primary, Statistic other) {
    map<string, int> primaryBookRates = primary.getBookRates();
    map<string, int> otherBookRates = other.getBookRates();

    set<string> commonBookRates;
    for(map<string, int>::iterator it = primaryBookRates.begin(); it != primaryBookRates.end(); ++it) {
        if(otherBookRates.find(it->first) != otherBookRates.end()) {
            commonBookRates.insert(it->first);
        }
    }

    if(commonBookRates.size() == 0) {
        return 0;
    }

    double result = 0;
    double primaryMeanRating = 0;
    double otherMeanRating = 0;
    int primaryMeanSum = 0;
    int otherMeanSum = 0;
    for(map<string, int>::iterator it = primaryBookRates.begin(); it != primaryBookRates.end(); ++it) {
        primaryMeanSum += it->second;
    }
    for(map<string, int>::iterator it = otherBookRates.begin(); it != otherBookRates.end(); ++it) {
        otherMeanSum += it->second;
    }
    if(primaryBookRates.size() > 0) {
        primaryMeanRating = (double) primaryMeanSum / primaryBookRates.size();
    }

    if(otherBookRates.size() > 0) {
        otherMeanRating = (double) otherMeanSum / otherBookRates.size();
    }

    double numerator = 0;
    double firstDenominator = 1;
    double secondDenominator = 1;

    for(set<string>::iterator it = commonBookRates.begin(); it != commonBookRates.end(); ++it) {
        numerator += (double)(primaryBookRates[*it] - primaryMeanRating) * (otherBookRates[*it] - otherMeanRating);
        firstDenominator += (double) (primaryBookRates[*it] - primaryMeanRating) * (primaryBookRates[*it] - primaryMeanRating);
        secondDenominator += (double) (otherBookRates[*it] - otherMeanRating) * (otherBookRates[*it] - otherMeanRating);
    }

    return (double) numerator / (sqrt(firstDenominator) * sqrt(secondDenominator));
}

// the spearman distance has a weight of 20% while the pearson correlation, based on rates, has a weight of 80%.
double Recommender::computeUserDistance(Statistic primary, Statistic other) {
    return (computePearsonCorrelation(primary, other) * 4 + computeSpearmanDistance(primary, other)) / 5;
}

Recommender::Recommender(string username) {
    this->username = username;
    Statistic aux(username);
    primary = aux;
    list<string> otherUsers = DatabaseQueries::getOtherActiveUsers(username);
    //--------------------- debug-----------------------------------
    std::cout << "Active users: " << std::endl;
    for(list<string>::iterator it = otherUsers.begin(); it != otherUsers.end(); ++it) {
        cout << *it << '\n';
    }

    // -------------------------DEBUG-----------------------------------
    for(list<string>::iterator it = otherUsers.begin(); it != otherUsers.end(); ++it) {
        Statistic stat(*it);
        otherStatistics[*it] = stat;
    }
    // Now we have primary Statistic and the other statistics
    // We have to compute user distances
    for(map<string, Statistic>::iterator it = otherStatistics.begin(); it != otherStatistics.end(); ++it) {
        userDistances[it->first] = computeUserDistance(primary, it->second);
    }
    // Now we have to fill the unreadRatedBooks member.

    for(map<string, Statistic>::iterator it = otherStatistics.begin(); it != otherStatistics.end(); ++it) {
        map<string, int> otherBookRates = it->second.getBookRates();
        for(map<string, int>::iterator it2 = otherBookRates.begin(); it2 != otherBookRates.end(); ++it2) {
            if(primary.getBookRates().find(it2->first) == primary.getBookRates().end()) {
                unreadRatedBooks.insert(it2->first);
            }
        }
    }
}

double Recommender::getAverageRate(Statistic stat) {
    double sum;
    for(map<string, int>::iterator it = stat.getBookRates().begin(); it != stat.getBookRates().end(); ++it) {
        sum += it->second;
    }
    return stat.getBookRates().size() == 0 ? 0 : (double)sum / stat.getBookRates().size();
}


list<string> Recommender::recommendBooks() {
    double primaryMeanRating = 0;

    if(primary.getBookRates().size() > 0) {
        int ratesSum = 0;
        for(map<string, int>::iterator it = primary.getBookRates().begin(); it != primary.getBookRates().end(); ++it) {
            ratesSum += it->second;
        }
        primaryMeanRating = (double) ratesSum / primary.getBookRates().size();
    }

    double denominator = 0;
    for(map<string, double>::iterator it = userDistances.begin(); it != userDistances.end(); ++it) {
        denominator += it->second;
    }

    vector<BookScore> bookByScores;
    for(set<string>::iterator it = unreadRatedBooks.begin(); it != unreadRatedBooks.end(); ++it) {
        // We assign a score to each book
        double denominator = 0;
        double numerator = 0;
        for(map<string, double>::iterator it2 = userDistances.begin(); it2 != userDistances.end(); ++it2) {
            string otherUsername = it2->first;
            if(otherStatistics[username].getBookRates().find(*it) != otherStatistics[username].getBookRates().end()) {
                // if otherUser rated the book;
                denominator += it2->second > 0 ? it2->second : (it2->second);
                numerator += (it2->second) * (otherStatistics[username].getBookRates()[*it] -
                                                getAverageRate(otherStatistics[username]));
            }
        }
        if(denominator != 0) {
            BookScore bookScore(*it, (double)primaryMeanRating + (double)(numerator / denominator));
            bookByScores.push_back(bookScore);
        }
    }

    sort(bookByScores.begin(), bookByScores.end(), BookScore::comparator);
    list<string> result;
    for(int i = 0; i < bookByScores.size(); ++i) {
        if(bookByScores[i].getScore() > SCORE_THRESHOLD) {
            result.push_back(bookByScores[i].getBook());
        }
    }

    if(result.size() < TOP_WANTED) {
        // we have to complete them
        list<string> completation = DatabaseQueries::getTopBooks(TOP_WANTED - result.size());
        for(list<string>::iterator it = completation.begin(); it != completation.end(); ++it) {
            result.push_back(*it);
        }
    }

    return result;
}
