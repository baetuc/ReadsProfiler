#include <string>
#include <User.h>

#pragma once

using namespace std;

string User::getUsername() {return username;}
string User::getPassword() {return password;}
void User::setUsername(string username) {this->username=username;}
void User::setPassword(string password) {this->password = password;}
