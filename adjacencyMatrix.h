#ifndef ADJACENCYMATRIX_h
#define ADJACENCYMATRIX_h

#define MAXNUMVERTEX 100

typedef int vertex;
typedef float weight;

typedef struct {
  weight matrix[MAXNUMVERTEX][MAXNUMVERTEX];
  int numVertex;
} Graph;

//All these functions returns a negative value when there's an error.
//The graph considers that the vertex counting start at zero.
int initGraph(Graph *g, int numVertex); //Initializes a graph.
int insertLine(Graph *g, vertex u, vertex v, weight value); //Insert a line between vertices u and v.
int checkLine(Graph *g, vertex u, vertex v); //Checks if there's a line between vertex u and v.
int removeLine(Graph *g, vertex u, vertex v); //Removes a line between two vertices.
int checkIfThereIsANeighboor(Graph *g, vertex u); //Checks if there's a connection with vertex u.
int getFirstNeighboor(Graph *g, vertex u); //Returns one vertex that is connected with the vertex u.

//Utils
void printGraph(Graph *g); //Shows a representation of graph

#endif
