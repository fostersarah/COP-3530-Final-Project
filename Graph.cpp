#include <vector>
#include <string>
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
