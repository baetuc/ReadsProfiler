#include <string>
#include <vector>
#include <climits>
#include <iostream>
#include <stdio.h>
#pragma once

using namespace std;

class Utility {
public:
    static string getStringForNumber(int number);
    static string getStringForFloat(float number);
    static vector<string> splitString(string source, char delimiter, int maxParts);
};

// Method that transforms an int to a string
string Utility::getStringForNumber(int number) {
    char length[5];
    snprintf(length, 5, "%d", number);
    string response(length);
    return response;
}

//Method that transforms a float into a string
string Utility::getStringForFloat(float number) {
    char length[6];
    snprintf(length, 6, "%f", number);
    string response(length);
    return response;
}

/* Method that splits a string by a delimiter, resulting maximum maxParts
 substrings. If maxParts is 0, then the number of substrings is maximal. */
vector<string> Utility::splitString(string source, char delimiter, int maxParts) {
    vector<string> result;
    string current;
    if(maxParts < 0) {
        return result;
    }
    // make maxParts take the maximum int value if it is 0.
    maxParts = maxParts == 0 ? INT_MAX : maxParts;
    int i = 0;
    bool isEmpty = true;
    while (maxParts > 1 && i < source.size()) {
        while (source[i] != delimiter && i < source.size()) {
            current += source[i++];
            isEmpty = false;
        }
        if(!isEmpty) {
            result.push_back(current);
            current.clear();
            isEmpty = true;
            --maxParts;
        }
        else {
            ++i;
        }
    }
    // we delete the delimiters at the beginning of the rest
    while(source[i] == delimiter) {
        ++i;
    }
    // we push the last string as the rest of the initial string
    while(i < source.size()) {
        current += source[i++];
    }
    if(!current.empty()) {
        result.push_back(current);
    }
    return result;
}
