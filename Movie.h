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
    int ID;
    
public:
    Movie(string& title, string& description, int ID, vector<string>& keywords);
    string getTitle();
    string getDescription();
    vector<string> getGenre();
    int getID();
};


#endif //COP3530FINALPROJECT_MOVIE_H
