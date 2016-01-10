#include <string>
#include <Author.h>
#pragma once

using namespace std;

string Author::getFirstName() {return firstName;}
string Author::getSecondName() {return secondName;}
void Author::setFirstName(string firstName) {this->firstName = firstName;}
void Author::setSecondName(string secondName) {this->secondName = secondName;}
