#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
using namespace std;

int main() {
    //need number of verticies
    string currentLine;
    ifstream inputFile;
    string currentWord;
    string genres;
    string trash;
    string ID;
    string description;
    string name;
    inputFile.open("genres-Table 1.csv");
    unordered_set<string> listGenres;
    getline(inputFile, currentLine);
    if (inputFile.is_open()) {
        while(getline(inputFile, currentLine)) {
            istringstream currentStream(currentLine);

            getline(currentStream, trash, '"');
            getline(currentStream, genres, '"');
//            getline(currentStream, genres, '"');
//            getline(currentStream, trash, ',');
//            getline(currentStream, ID, ',');
//            getline(currentStream, trash, '"');
//            getline(currentStream, description, '"');
//            getline(currentStream, trash, ',');
//            getline(currentStream, name);

            istringstream  genreStream(genres);
            string currentGenre;
            while(getline(genreStream, currentGenre, ',')) {
                string pushGenre;
                try {
                    stoi(currentGenre);
                }
                catch(exception &e) {
                    istringstream oneGenre(currentGenre);
                    getline(oneGenre, trash, '\'');
                    getline(oneGenre,pushGenre, '\'');
                    listGenres.insert(pushGenre);
                }
            }
        }
    }

//    for (auto it = listGenres.begin(); it != listGenres.end(); it++ ) {
//        cout << *it;
//    }



    return 0;
}