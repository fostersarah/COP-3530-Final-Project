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
    int ID;
    vector<string> keywords;
public:
    Movie(string& title, string& description, int ID, vector<string>& keywords);
    string getTitle();
    string getDescription();
    int getID();
    vector<string> getKeywords();
};


#endif //COP3530FINALPROJECT_MOVIE_H
