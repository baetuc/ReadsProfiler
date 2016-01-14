#include <cmath>
#include "Statistic.h"
#include "BookScore.h"
#include <list>
#include <algorithm>
#include <vector>
#include "SerializerDeserializer.h"

#define TOP_WANTED 10
#define SCORE_THRESHOLD 0

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
    map<string, int> primaryGenreRank = primary.getGenreRank();
    map<string, int> otherGenreRank = other.getGenreRank();

    int primaryBiggestRank = 0;
    int otherBiggestRank = 0;

    set<string> impliedGenres;
    for(map<string, int>::iterator it = primaryGenreRank.begin(); it != primaryGenreRank.end(); ++it) {
        impliedGenres.insert(it->first);
        primaryBiggestRank = it->second > primaryBiggestRank ? it -> second : primaryBiggestRank;
    }
    for(map<string, int>::iterator it = otherGenreRank.begin(); it != otherGenreRank.end(); ++it) {
        impliedGenres.insert(it->first);
        otherBiggestRank = it->second > otherBiggestRank ? it -> second : otherBiggestRank;
    }

    ++primaryBiggestRank;
    ++otherBiggestRank;

    if(impliedGenres.size() < 2) {
        return 0;
    }

    double squareSum = 0;

    for(set<string>::iterator it = impliedGenres.begin(); it != impliedGenres.end(); ++it) {
        int primaryFactor = primaryGenreRank.find(*it) != primaryGenreRank.end() ? primaryGenreRank[*it] : primaryBiggestRank;
        int otherFactor = otherGenreRank.find(*it) != otherGenreRank.end() ? otherGenreRank[*it] : otherBiggestRank;
        cout << "Spearman " << " " << *it << " primary: " << primaryFactor << ", " << other.getUsername() << ": ";
        cout << otherFactor << '\n';
        squareSum += (primaryFactor - otherFactor) * (primaryFactor - otherFactor);
    }
    cout << "Square sum: " << squareSum << "\nn is: " << impliedGenres.size() << '\n';
    return 1 - ((double)(6 * squareSum) / (impliedGenres.size() * (impliedGenres.size() * impliedGenres.size() - 1)));
}

double Recommender::computeSubgenreSpearmanDistance(Statistic primary, Statistic other) {
    map<string, int> primarySubgenreRank = primary.getSubgenreRank();
    map<string, int> otherSubgenreRank = other.getSubgenreRank();

    int primaryBiggestRank = 0;
    int otherBiggestRank = 0;

    set<string> impliedSubgenres;
    for(map<string, int>::iterator it = primarySubgenreRank.begin(); it != primarySubgenreRank.end(); ++it) {
        impliedSubgenres.insert(it->first);
        primaryBiggestRank = it->second > primaryBiggestRank ? it -> second : primaryBiggestRank;
    }
    for(map<string, int>::iterator it = otherSubgenreRank.begin(); it != otherSubgenreRank.end(); ++it) {
        impliedSubgenres.insert(it->first);
        otherBiggestRank = it->second > otherBiggestRank ? it -> second : otherBiggestRank;
    }

    ++primaryBiggestRank;
    ++otherBiggestRank;

    if(impliedSubgenres.size() < 2) {
        return 0;
    }

    double squareSum = 0;

    for(set<string>::iterator it = impliedSubgenres.begin(); it != impliedSubgenres.end(); ++it) {
        int primaryFactor = primarySubgenreRank.find(*it) != primarySubgenreRank.end() ?
                                                                        primarySubgenreRank[*it] : primaryBiggestRank;
        int otherFactor = otherSubgenreRank.find(*it) != otherSubgenreRank.end() ? otherSubgenreRank[*it] : otherBiggestRank;
        cout << "Spearman " << " " << *it << " primary: " << primaryFactor << ", " << other.getUsername() << ": ";
        cout << otherFactor << '\n';
        squareSum += (primaryFactor - otherFactor) * (primaryFactor - otherFactor);
    }
    cout << "Square sum: " << squareSum << "\nn is: " << impliedSubgenres.size() << '\n';
    return 1 - ((double)(6 * squareSum) / (impliedSubgenres.size() * (impliedSubgenres.size() * impliedSubgenres.size() - 1)));
}

double Recommender::computeAuthorSpearmanDistance(Statistic primary, Statistic other) {
    map<int, int> primaryAuthorRank = primary.getAuthorRank();
    map<int, int> otherAuthorRank = other.getAuthorRank();

    int primaryBiggestRank = 0;
    int otherBiggestRank = 0;

    set<int> impliedAuthors;
    for(map<int, int>::iterator it = primaryAuthorRank.begin(); it != primaryAuthorRank.end(); ++it) {
        impliedAuthors.insert(it->first);
        primaryBiggestRank = it->second > primaryBiggestRank ? it -> second : primaryBiggestRank;
    }
    for(map<int, int>::iterator it = otherAuthorRank.begin(); it != otherAuthorRank.end(); ++it) {
        impliedAuthors.insert(it->first);
        otherBiggestRank = it->second > otherBiggestRank ? it -> second : otherBiggestRank;
    }

    ++primaryBiggestRank;
    ++otherBiggestRank;

    if(impliedAuthors.size() < 2) {
        return 0;
    }

    double squareSum = 0;

    for(set<int>::iterator it = impliedAuthors.begin(); it != impliedAuthors.end(); ++it) {
        int primaryFactor = primaryAuthorRank.find(*it) != primaryAuthorRank.end() ? primaryAuthorRank[*it] : primaryBiggestRank;
        int otherFactor = otherAuthorRank.find(*it) != otherAuthorRank.end() ? otherAuthorRank[*it] : otherBiggestRank;
        cout << "Spearman " << " " << *it << " primary: " << primaryFactor << ", " << other.getUsername() << ": ";
        cout << otherFactor << '\n';
        squareSum += (primaryFactor - otherFactor) * (primaryFactor - otherFactor);
    }
    cout << "Square sum: " << squareSum << "\nn is: " << impliedAuthors.size() << '\n';
    return 1 - ((double)(6 * squareSum) / (impliedAuthors.size() * (impliedAuthors.size() * impliedAuthors.size() - 1)));
}

double Recommender::computeSpearmanDistance(Statistic primary, Statistic other) {
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
    primary = Statistic(username);

    map<string, int> hello = primary.getBookRates();
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

    unreadRatedBooks = DatabaseQueries::getUnreadRatedBooks(username);

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
            map<string, int> otherBookRates = otherStatistics[otherUsername].getBookRates();
            if(otherBookRates.find(*it) != otherBookRates.end()) {
                // if otherUser rated the book;
                denominator += it2->second > 0 ? it2->second : (-it2->second);
                numerator += (it2->second) * (otherBookRates[*it] - getAverageRate(otherStatistics[otherUsername]));

                cout << username << " " << it2->first << " distance is: " << it2->second << '\n';
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
        cout << "Selected: " << bookByScores[i].getBook() << " " << bookByScores[i].getScore() << '\n';
        if(bookByScores[i].getScore() > SCORE_THRESHOLD) {
            result.push_back(bookByScores[i].getBook());
        }
    }

    cout << "Recommended books size: " << result.size() << '\n';
    if(result.size() < TOP_WANTED) {
        // we have to complete them
        list<string> completation = DatabaseQueries::getTopBooks(TOP_WANTED - result.size());
        for(list<string>::iterator it = completation.begin(); it != completation.end(); ++it) {
            result.push_back(*it);
            cout << "From top: " << *it << '\n';
        }
    }

    return result;
}
