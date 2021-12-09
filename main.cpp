#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <chrono>
#include <algorithm>
using namespace std;

struct Edge
{
    int src, dest; // movie names
    int weight; // weight between movies
    Edge(int _src, int _dest, int _weight)
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
    vector<vector<pair<int,int>>> adjList;
    // e.g. adjList["Toy Story"] = {("Finding Nemo", 40), ("Toy Story 2", 100)}

    Graph(const vector<Edge*>& edges, int vertices)
    {
        numVertices = vertices;

        adjList.resize(vertices);

        for (auto &edge : edges)
        {
            adjList[edge->src].push_back(make_pair(edge->dest, edge->weight));
        }
    }
};

class Matrix {
public:
    vector<vector<int>> matrix;
    int numVertices;
    Matrix(int vertices) 
    {
        numVertices = vertices;
        for (int i = 0; i < 2000; i++)
        {
            vector<int> row;
            for (int j = 0; j < 2000; j++)
            {
                row.push_back(0);
            }
            matrix.push_back(row);
        }
    }
    void insertWeight(int to, int from, int weight) {
        matrix[to][from] = weight;
        matrix[from][to] = weight;
    }
    int getWeight(int to, int from) {
        return matrix[to][from];
    }
    bool areAdjacent(int to, int from) {
        if (!matrix[to][from]) {
            return true;
        }
        else {
            return false;
        }
    }
    int getNumVertices()
    {
        return numVertices;
    }
};

class Movie {
    string title;
    string description;
    vector<string> genre;
    int index;

public:
    Movie();
    Movie(string& title, string& description, vector<string>& genre, int index);
    string getTitle();
    string getDescription();
    int getIndex();
    vector<string> getGenre();
};
Movie::Movie() {
    title = "";
    description = "";
    vector<string> genres;
    genre = genres;
    index = 0;
}

Movie::Movie(string& title, string& description, vector<string>& genre, int index) {
    this->title = title;
    this->description = description;
    this->genre = genre;
    this->index = index;
}
string Movie::getDescription() {
    return description;
}
string Movie::getTitle() {
    return title;
}
int Movie::getIndex() {
    return index;
};

vector<string> Movie::getGenre() {
    return genre;
}

int calculateWeight(Movie movie1, Movie movie2) {
	set<string> unique;
	vector<string> movie1Genres = movie1.getGenre();
	vector<string> movie2Genres = movie2.getGenre();
	
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

vector<Edge*> makeEdges(map<string, vector<Movie>>& movies, vector<string>& genres) 
{
    unordered_set<string> completed;
    vector<Edge*> edges;
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

                Edge* edge1 = new Edge(movies[genres[i]][j].getIndex(), movies[genres[i]][k].getIndex(), weight);
                Edge* edge2 = new Edge(movies[genres[i]][k].getIndex(), movies[genres[i]][j].getIndex(), weight);


                edges.push_back(edge1);
                edges.push_back(edge2);
	    }
	}
    }
    return edges;
}

void makeMatrix(map<string, vector<Movie>>& movies, vector<string>& genres, Matrix& matrix) // O(gn^2) time where g is num genres and n is num movies per genre
{
    unordered_set<string> completed;
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

                matrix.insertWeight(movies[genres[i]][j].getIndex(), movies[genres[i]][k].getIndex(), weight);
            }
        }
    }
}

vector<int> dijkstraList(const Graph& graph, int src) {
    int s = src;

    int vertices = graph.numVertices;
    vector<int> distance;
    for (int i = 0; i < vertices; i++)
    {
        distance.push_back(INT_MAX);
    }
    distance[src] = 0;
    int previous[vertices];
    for (int i = 0; i < vertices; i++)
    {
        previous[i] = -1;
    }

    set<int> computed;
    set<int> processing;
    for (int i = 0; i < vertices; i++) 
    {
        processing.insert(i);
    }
    computed.insert(src);
    processing.erase(src);

    while (!processing.empty()) 
    {
        for (int i = 0; i < graph.adjList[s].size(); i++)
        {
            if (distance[graph.adjList[s][i].first] > distance[s] + graph.adjList[s][i].second)
            {
                distance[graph.adjList[s][i].first] = distance[s] + graph.adjList[s][i].second;
                previous[graph.adjList[s][i].first] += 1;
            }
        }
        computed.insert(s);
        processing.erase(s);

        int min = INT_MAX;
        for (int i = 0; i < vertices; i++)
        {
            if (distance[i] < min && computed.count(i) == 0)
            {
                min = distance[i];
                s = i;
            }
        }
    }
    return distance;
}

vector<int> dijkstraMatrix(const Matrix& arr, int src) {
    int vertices = arr.numVertices;
    vector<int> distance;
    set<int> included;

    for (int i = 0; i < arr.numVertices; i++)
    {
        distance.push_back(INT_MAX);
    }

    distance[src] = 0;

    for (int i = 0; i < vertices; i++) 
    {
        int min = INT_MAX;
        int minIndex;

        for (int j = 0; j < vertices; j++)
        {
            if (included.count(j) == 0 && distance[j] <= min)
            {
                min = distance[j];
                minIndex = j;
            }
        }
        included.insert(minIndex);
        for (int j = 0; j < vertices; j++)
        {
            if (!included.count(j) && arr.matrix[minIndex][j] && distance[minIndex] != INT_MAX && distance[minIndex] + arr.matrix[minIndex][j] < distance[j])
            {
                distance[j] = distance[minIndex] + arr.matrix[minIndex][j];
            }
        }
    }
    return distance;
}

int main() {
    unordered_set<string> listGenres; //all possible genres
    unordered_set<string> listMovies;
    
    vector<string> allGenres;
    map<string, vector<Movie>> moviesByGenre;
    map<string, Movie> catalogue;
    vector<Movie> catalogueVector;
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
        && getline(inputFile3, currentLine3) && totalMovies < 2000) {

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

            description = currentLine2;
            title = currentLine3;
            listMovies.insert(title);

            Movie tempMovie =  Movie(title, description, movieGenres, totalMovies); 
            
            catalogue[title] = tempMovie;
            catalogueVector.push_back(tempMovie);
            
            totalMovies++;
            
            for (int i = 0; i < tempMovie.getGenre().size(); i++)
            {
                moviesByGenre[tempMovie.getGenre()[i]].push_back(tempMovie);
            }
        }
    }

    cout << " THE BLUE TEAM PRESENTS!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << " Watch Next For..." << endl << endl;

    vector<Edge*> edges = makeEdges(moviesByGenre, allGenres);
    Graph adjList = Graph(edges, totalMovies);

    Matrix matrixList(totalMovies);
    makeMatrix(moviesByGenre, allGenres, matrixList);

    bool keepRunning = true;

    while (keepRunning)
    {
        cout << " Enter Movie Name: ";
        string userTitle;
        getline(cin, userTitle);

        int userIndex = 0;

        if (!listMovies.count(userTitle))
        {
            cout << " " << userTitle << " is not in our dataset" << endl;
            continue;
        }
        else
        {
            userIndex = catalogue[userTitle].getIndex();
        }

        cout << " What Graph Structure Should Be Used? (list/matrix) ";
        string graphType;
        cin >> graphType;

        bool isList = false;
        bool isMatrix = false;

        if (graphType.compare("list") == 0)
        {
            isList = true;
            isMatrix = false;
        }
        else
        {
            isList = false;
            isMatrix = true;
        }

        vector<int> shortestList;

        auto start = chrono::steady_clock::now();

        int indexMax = 0;
        int max = 0;

        if (isList)
        {
            shortestList = dijkstraList(adjList, userIndex);
            for (int i = 0; i < shortestList.size(); i++) {
                if (shortestList[i] > max) {
                    indexMax = i;
                    max = shortestList[i];
                }
            }
        }
        else if (isMatrix)
        {
            shortestList = dijkstraMatrix(matrixList, userIndex);
            for (int i = 0; i < shortestList.size(); i++)
            {
                if (shortestList[i] > max && shortestList[i] != INT_MAX)
                {
                    indexMax = i;
                    max = shortestList[i];
                }
            }
        }

        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;

        cout << " The Furthest Movie From " << catalogueVector[userIndex].getTitle() << " is:" << endl;

        cout << " Title: " << catalogueVector[indexMax].getTitle() << endl;
        cout << " Genre: ";
        for (int i = 0; i < catalogueVector[indexMax].getGenre().size(); i++)
        {
            if (i != catalogueVector[indexMax].getGenre().size() - 1)
            {
                cout << catalogueVector[indexMax].getGenre()[i] << ", ";
            }
            else
            {
                cout << catalogueVector[indexMax].getGenre()[i] << endl;
            }
        }
        cout << " Description: " << catalogueVector[indexMax].getDescription() << endl;
        cout << endl;
        cout << " Time to Run: " << chrono::duration <double, std::milli>(diff).count() << " ms" << endl;

        cout << " Run again? (1/0) ";
        char yesNo;
        cin >> yesNo;
        cin.ignore();
        keepRunning = yesNo-48;
    }
   
    return 0;
}
