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
vector<string> Movie::getGenre() {
    return genre;
}
Movie::Movie(string& title, string& description, vector<string>& genre) {
    this->title = title;
    this->description = description;
    this->genre = genre;
}

