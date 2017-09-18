#ifndef ADJACENCYMATRIX_h
#define ADJACENCYMATRIX_h

#define MAXNUMVERTEX 100

typedef int block;
typedef float weight;

typedef struct {
  weight matrix[MAXNUMVERTEX][MAXNUMVERTEX];
  int numVertex;
} Graph;

//All these functions returns a negative value when there's an error.
//The graph considers that the vertex counting start at zero.
void initGraph(Graph *g, int numVertex); //Initializes a graph.
int insertLine(Graph *g, block u, block v, weight value); //Insert a line.
int checkLine(Graph *g, block u, block v); //Checks if there's a line between vertex u and v.
int removeLine(Graph *g, block u, block v); //Removes a line between two vertex.
int checkIfThereIsANeighboor(Graph *g, block u); //Checks if there's a connection with vertex u.
int getFirstNeighboor(Graph *g, block u); //Returns one vertex that is connected with the vertex u.

//Utils
void printGraph(Graph *g); //Shows a representation of graph

#endif
