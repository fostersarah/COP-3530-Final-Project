# COP-3503-Final-Project

Organizing Github
File I/O
Building movie class
Building graph representation (Adjacency Matrix)
Building graph representation (Adjacency List)
Building GUI interface

Strategy:
Use 2 different graph implementations to compare their performance. We will use an adjacency
matrix and adjacency list. The graph will contain vertices of an abstract data type called “Movie”
which will contain different attributes such as title, movie description, and rating. The structure
will also contain up to 3 given attributes of a movie. In order to connect movies to one another,
the edges will be weighted as a ranking of how similar the movies are to one another. 1 would
be most similar and 5 would be completely different. This would allow us to use Dijkstra’s
shortest path algorithm to find similar movies to the movie first inputted by the user. The graph
would be stored in an unordered map, that way the key being the movie name could be

Kaggle movie dataset
https://www.kaggle.com/rounakbanik/the-movies-dataset?select=movies_metadata.csv
Vertices: the different movies in the database
Edges: The similarity of the movies based on number of shared keywords and genres
