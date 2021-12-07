

struct Edge
{
	string src, dest;
	int weight;
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
	int numVertices;
	map <string, vector<pair<string,int>>> adjList;

	Graph(const vector<Edge>& edges, int vertices)
	{
		numVertices = vertices;
		for (auto &edge : edges)
		{
			adjList[edge.src].push_back(make_pair(edge.dest, edge.weight));
		}
	}
};
