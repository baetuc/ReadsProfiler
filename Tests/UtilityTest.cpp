#include "Utility.h"
#include <string>
#include <list>
#include <iostream>

using namespace std;

int main() {
    int response;
    cout << "Please insert 1 if you want to continue, 0 if you want to finish: ";
    cin >> response;
    while (response) {
        string current;
        cout << "Please insert a string: ";
        cin >> current;
        char delimiter;
        cout << "Please insert a delimiter: ";
        cin >> delimiter;
        int maxParts;
        cout << "Please insert maximum words splitted: ";
        cin >> maxParts;

        list<string> result = Utility::splitString(current, delimiter, maxParts);
        cout << "The result length is: " << result.size() << "\n";
        for(list<string>::iterator it = result.begin(); it != result.end(); ++it) {
            cout << (*it) << '\n';
        }

        cout << "Please insert 1 if you want to continue, 0 if you want to finish: ";
        cin >> response;
    }
}
