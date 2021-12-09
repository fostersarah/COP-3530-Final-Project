#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <chrono>
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
    Graph(){
        numVertices = 0;
    }
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
    vector<vector<int>> matrix;
    int numVertices;
public:
    Matrix(){
        numVertices = 0;
    }
    Matrix(int vertices)
    {
        numVertices = vertices;
        for (int i = 0; i < 2000; i++)
        {
            vector<int> row;
            for (int j = 0; j < 2000; j++)
            {
                row.push_back(j);
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
public:
    string title;
    string description;
    vector<string> genre;
    int index;

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

int calculateWeight(Movie movie1, Movie movie2) {

    set<string> unique;
    vector<string> movie1Genres = movie1.genre;
    vector<string> movie2Genres = movie2.genre;

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
    //for (int i = 0; i < 1; i++)
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

void makeMatrix(map<string, vector<Movie>>& movies, vector<string>& genres, Matrix& matrix)
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
    vector<int> previous(vertices);
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
    cout << "before processing" << endl;
    int check = 0;
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
        // find new s by making sure it is the smallest in distance but not in computed

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
    cout << "after processing" << endl;

    return distance;
}

vector<int> dijkstraMatrix(Matrix& arr, int src) {
    int s = src;

    int vertices = arr.getNumVertices();
    vector<int> distance;
    for (int i = 0; i < vertices; i++)
    {
        distance.push_back(INT_MAX);
    }
    distance[s] = 0;
    vector<int> previous(vertices);
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
    computed.insert(s);
    processing.erase(s);

    while (!processing.empty())
    {
        for (int i = 0; i < 2000; i++)
        {

            if (distance[i] > distance[s] + arr.getWeight(s, i))
            {
                distance[i] = distance[s] + arr.getWeight(s,i);
                previous[i] += 1;
            }
        }
        computed.insert(s);
        processing.erase(s);
        // find new s by making sure it is the smallest in distance but not in computed

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
Graph adjList;
Matrix adjMatrix;
map<string, Movie> catalogue;
vector<Movie> catalogueVector;
vector<int> shortestList;
vector<int> shortestMatrix;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->hide();
    //vector<Movie> movies; //every movie in the list
       unordered_set<string> listGenres; //all possible genres

       vector<string> allGenres;
       map<string, vector<Movie>> moviesByGenre;

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

                  Movie tempMovie =  Movie(title, description, movieGenres, totalMovies); //make a new movie
                  //movies.push_back(tempMovie); //store movie in vector

                  catalogue[title] = tempMovie;
                  catalogueVector.push_back(tempMovie);

                  totalMovies++;

                  for (int i = 0; i < tempMovie.genre.size(); i++)
                  {
                      moviesByGenre[tempMovie.genre[i]].push_back(tempMovie);
                  }
              }
          }
       cout << totalMovies << endl;

       vector<Edge*> edges = makeEdges(moviesByGenre, allGenres);

       cout << "edges" << endl;

       adjList = Graph(edges, totalMovies);

       //shortestList = dijkstraList(adjList, 0);

       cout << "list" << endl;

       Matrix test(totalMovies);
       //cout from 0 to 1
       adjMatrix = test;
       makeMatrix(moviesByGenre, allGenres, adjMatrix);

       shortestMatrix = dijkstraMatrix(test, 0);

       cout << "matrix" << endl;

       cout << "done";
}

void MainWindow::on_pushButton_3_clicked()
{
    //adjMatrix
    ui->listWidget->clear();

    QString temp1 = ui->lineEdit->text();

    int index = catalogue[temp1.toStdString()].getIndex();

    int indexMax = 0;
    int max = 0;
    for (int i = 0; i < shortestMatrix.size(); i++)
    {
        if (shortestMatrix[i] > max && shortestMatrix[i] != INT_MAX)
        {
            indexMax = i;
            max = shortestMatrix[i];
        }
    }
    ui->listWidget->addItem(QString::fromStdString(catalogueVector[indexMax].title));



}

void MainWindow::on_pushButton_2_clicked()
{

    //adjList
    ui->listWidget->clear();

    QString temp1 = ui->lineEdit->text();

    int index = catalogue[temp1.toStdString()].getIndex();

    int indexMax = 0;
    int max = 0;

    for (int i = 0; i < shortestList.size(); i++) {
        if (shortestList[i] > max) {
            indexMax = i;
            max = shortestList[i];
        }
    }
    ui->listWidget->addItem(QString::fromStdString(catalogueVector[indexMax].title));
}

void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
    QString temp = ui->listWidget->currentItem()->text();
    string title = temp.toStdString();

    string description = catalogue[temp.toStdString()].getDescription();

    QMessageBox::information(this, tr(title.c_str()), tr(description.c_str()));
}
