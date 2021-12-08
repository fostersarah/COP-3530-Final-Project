#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
using namespace std;

class Movie {
    string title;
    string description;
    vector<string> genre;

public:
    Movie();
    Movie(string& title, string& description, vector<string>& genre);
    string getTitle();
    string getDescription();
    vector<string> getGenre();
};
Movie::Movie() {
    title = "";
    description = "";
    vector<string> genres;
    genre = genres;
}

Movie::Movie(string& title, string& description, vector<string>& genre) {
    this->title = title;
    this->description = description;
    this->genre = genre;
}
string Movie::getDescription() {
    return description;
}
string Movie::getTitle() {
    return title;
}
vector<string> Movie::getGenre() {
    return genre;
}

int main() {
    vector<Movie> movies; //every movie in the list
    unordered_set<string> listGenres; //all possible genres
    
    vector<string> allGenres;
    map<string, vector<Movie>> moviesByGenre;
    map<string, Movie> catalogue;
    int totalMovies = 0;

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
                
                if (listGenres.count(currentGenre) == 0)
                {
                    allGenres.push_back(currentGenre);
                }
            }

            istringstream currentStream2(currentLine2); //opens 2nd file to get the description/title

            getline(currentStream2, trash, '"');
            getline(currentStream2, description, '"');
            getline(currentStream2,trash, ',');
            getline(currentStream2, title);

            Movie tempMovie =  Movie(title, description, movieGenres); //make a new movie
            movies.push_back(tempMovie); //store movie in vector
            
            catalogue[title] = tempMovie;
            
            totalMovies++;
            
            for (int i = 0; i < tempMovie.getGenre().size(); i++)
            {
                moviesByGenre[tempMovie.getGenre()[i]].push_back(tempMovie);
            }
        }
    }

    cout << totalMovies;

    return 0;
}
