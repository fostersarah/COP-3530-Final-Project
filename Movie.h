//
// Created by Sarah Foster on 12/6/21.
//
#include <string>
#include <vector>
using namespace std;
#ifndef COP3530FINALPROJECT_MOVIE_H
#define COP3530FINALPROJECT_MOVIE_H

class Movie {
    string title;
    string description;
    vector<string> genre;

public:
    Movie(string& title, string& description, vector<string>& keywords);
    string getTitle();
    string getDescription();
    vector<string> getGenre();
};


#endif //COP3530FINALPROJECT_MOVIE_H
