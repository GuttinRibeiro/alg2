#ifndef ADJACENCYMATRIX_h
#define ADJACENCYMATRIX_h
#include <float.h>

#define MAXNUMVERTEX 100
#define INF FLT_MAX
#define NINF FLT_MIN

typedef int vertex;
typedef float weight;

typedef struct {
  weight matrix[MAXNUMVERTEX][MAXNUMVERTEX];
  int numVertex;
} Graph;

//All these functions returns a non positive value when there's something not expected:
/*
  -1 : indicates that one of the parameters has a problem
   0 : indicates that the function hasn't been performed or that it logical value is equal to false
   1 : indicates that the function has been performed or that it logical value is equal to true
*/
//The graph considers that the vertex counting start at zero.
int initGraph(Graph *g, int numVertex); //Initializes a graph
int insertDirectedLine(Graph *g, vertex u, vertex v, weight value); //Inserts a directed line from vertex u to vertex v
int checkLine(Graph *g, vertex u, vertex v); //Checks if there's a line from vertex u to vertex v
int removeLine(Graph *g, vertex u, vertex v); //Removes a line from vertex u to vertex v
int checkIfThereIsANeighboor(Graph *g, vertex u); //Checks if there's a connection with vertex u.
int getFirstNeighboor(Graph *g, vertex u); //Returns one vertex that is connected with the vertex u.
weight getWeight(Graph *g, vertex u, vertex v);

//Utils
void printGraph(Graph *g); //Shows a representation of graph

#endif