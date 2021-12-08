#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Movie.h"
using namespace std;

int main() {
    unordered_map<string, Movie> moviesMap; //every movie in the list
    unordered_set<string> listGenres; //all possible genres
    vector<string> genresVector;
    vector<Movie> movieVector;
    int numMovies = 0;

    string currentLine; //file i/o
    string currentLine2;
    ifstream inputFile;
    ifstream inputFile2;

    string trash; //variables for getting info
    string genres;
    string description;
    string title;

    inputFile.open("genres-Table 1.csv"); //open files
    inputFile2.open("overviewtitle.csv");

    getline(inputFile, currentLine); //get rid of first line
    getline(inputFile2, currentLine2);

    if (inputFile.is_open() && inputFile2.is_open()) {
        while(getline(inputFile, currentLine) && getline(inputFile2, currentLine2)) {

            istringstream currentStream(currentLine);

            getline(currentStream, trash, '"'); //remove extraneous "" marks
            getline(currentStream, genres, '"');

            vector<string> movieGenres;

            while(genres.find(',') != -1) { //gets the genres of this one movie
                genres = genres.substr(genres.find(',') + 1, genres.size() - genres.find(',') + 1);
                string currentGenre = genres.substr(0, genres.find(','));
                movieGenres.push_back(currentGenre); //keeps track of this movies genres
                genres = genres.substr(genres.find(',') + 1, genres.size() - genres.find(',') + 1);

                listGenres.insert(currentGenre); //keeps track of every genre
                if (listGenres.count(currentGenre) == 0) {
                    genresVector.push_back(currentGenre);
                }
            }

            istringstream currentStream2(currentLine2); //opens 2nd file to get the description/title

            getline(currentStream2, trash, '"');
            getline(currentStream2, description, '"');
            getline(currentStream2,trash, ',');
            getline(currentStream2, title);

            Movie tempMovie = Movie(title, description, movieGenres); //make a new movie
            moviesMap[title] = tempMovie; //store movie in vector
            movieVector.push_back(tempMovie);

            numMovies++;

        }
    }

    for (int i = 0; i < genresVector.size(); i++) {
        cout << genresVector[i] << " ";
    }

    //int of all movies
    //map <name, movie>
    //

    return 0;
}