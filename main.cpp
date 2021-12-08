#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <algorithm>
using namespace std;

struct Edge
{
    string src, dest; // movie names
    int weight; // weight between movies
    Edge(string _src, string _dest, int _weight)
    {
        src = _src;
        dest = _dest;
        weight = _weight;
    }
};

class Graph
{
public:
    int numVertices; // number of movies
    map <string, vector<pair<string,int>>> adjList;
    // e.g. adjList["Toy Story"] = {("Finding Nemo", 40), ("Toy Story 2", 100)}

    Graph(const vector<Edge>& edges, int vertices)
    {
        numVertices = vertices;
        for (auto &edge : edges)
        {
            adjList[edge.src].push_back(make_pair(edge.dest, edge.weight));
        }
    }
};

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

int calculateWeight(Movie movie1, Movie movie2) {
	set<string> unique;
	vector<string> movie1Genres = movie1.getGenre();
	vector<string> movie2Genres = movie1.getGenre();
	
	for (int i = 0; i < movie1Genres.size(); i++) {
		unique.insert(movie1Genres[i]);
	}

	for (int i = 0; i < movie2Genres.size(); i++) {
		unique.insert(movie2Genres[i]);
	}


	sort(movie1Genres.begin(), movie1Genres.end());
	sort(movie2Genres.begin(), movie2Genres.end());

	vector<string> v(movie1Genres.size() + movie2Genres.size());
	vector<string>::iterator it;

	it = set_intersection(movie1Genres.begin(), movie1Genres.end(), movie2Genres.begin(), movie2Genres.end(), v.begin());

	v.resize(it - v.begin());

	float decimal = (float)1 - ((float)v.size() / (float)unique.size());

	int result = 100 * decimal;

	return result;

}

vector<Edge> makeEdges(map<string, vector<Movie>>& movies, vector<string>& genres)
{
    unordered_set<string> completed;
    vector<Edge> edges;
    for (int i = 0; i < genres.size(); i++)
    {
        for (int j = 0; j < movies[genres[i]].size(); j++)
        {
            for (int k = j+1; k < movies[genres[i]].size(); k++)
            {
                string orient1 = movies[genres[i]][j].getTitle() + movies[genres[i]][k].getTitle();
                string orient2 = movies[genres[i]][k].getTitle() + movies[genres[i]][j].getTitle();

                if (completed.count(orient1) != 0 || completed.count(orient2) != 0)
                {
                    continue;
                }
                else
                {
                    completed.insert(orient1);
                    completed.insert(orient2);
                }

                int weight = calculateWeight(movies[genres[i]][j], movies[genres[i]][k]);

                Edge edge1 = Edge(movies[genres[i]][j].getTitle(), movies[genres[i]][k].getTitle(), weight);
                Edge edge2 = Edge(movies[genres[i]][k].getTitle(), movies[genres[i]][j].getTitle(), weight);


                edges.push_back(edge1);
                edges.push_back(edge2);
	    }
	}
    }
    return edges;
    
    
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
    string currentLine3;
    ifstream inputFile;
    ifstream inputFile2;
    ifstream inputFile3;

    string trash; //variables for getting info
    string genres;
    string description;
    string title;

    inputFile.open("genres.csv"); //open files
    inputFile2.open("overview.csv");
    inputFile3.open("title.csv");


    getline(inputFile, currentLine); //get rid of first line
    getline(inputFile2, currentLine2);
    getline(inputFile3, currentLine3);

    if (inputFile.is_open() && inputFile2.is_open()) {
        while(getline(inputFile, currentLine) && getline(inputFile2, currentLine2)
        && getline(inputFile3, currentLine3)) {

            istringstream currentStream(currentLine);

            getline(currentStream, trash, '"'); //remove extraneous "" marks
            getline(currentStream, genres, '"');

            vector<string> movieGenres;

            while(genres.find(',') != -1) { //gets the genres of this one movie
                genres = genres.substr(genres.find(',') + 1, genres.size() - genres.find(',') + 1);
                string currentGenre = genres.substr(0, genres.find(','));
                movieGenres.push_back(currentGenre); //keeps track of this movies genres
                genres = genres.substr(genres.find(',') + 1, genres.size() - genres.find(',') + 1);
                if (listGenres.count(currentGenre) == 0)
                {
                    allGenres.push_back(currentGenre);
                }
                listGenres.insert(currentGenre); //keeps track of every genre
                

            }

//            istringstream currentStream2(currentLine2); //opens 2nd file to get the description/title
//
//            getline(currentStream2, trash, '"');
//            getline(currentStream2, description, '"');
//            getline(currentStream2,trash, ',');
//            getline(currentStream2, title);
            description = currentLine2;
            title = currentLine3;

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
	
    for (int i = 0; i < 1; i++)
    {
	    cout << allGenres[i] << ": ";
	    for (int j = 0; j < moviesByGenre[allGenres[i]].size(); j++)
	    {
		    cout << moviesByGenre[allGenres[i]][j].getTitle() << " ";
	    }
    }
		    

    return 0;
}
