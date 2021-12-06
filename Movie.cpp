//
// Created by Sarah Foster on 12/6/21.
//

#include "Movie.h"

string Movie::getDescription() {
    return description;
}
string Movie::getTitle() {
    return title;
}
int Movie::getID() {
    return ID;
}
vector<string> Movie::getKeywords() {
    return keywords;
}
Movie::Movie(string& title, string& description, int ID, vector<string>& keywords) {
    this->title = title;
    this->description = description;
    this->ID = ID;
    this->keywords = keywords;
}

