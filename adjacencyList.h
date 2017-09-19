#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#define MAXNUMVERTEX 100

typedef int vertex;
typedef float weight;

typedef struct node {
  vertex id;
  weight value;
  struct node *next;
} block;

typedef struct {
  block *list[MAXNUMVERTEX];
  int numVertex;
} Graph;

//All these functions returns a non positive value when there's an error
//The graph considers that the vertex counting start at zero.
int initGraph(Graph *g, int numVertex); //Initializes a graph
int finishGraph(Graph *g); //Finishes a graph
block *initBlock(vertex u, weight value); //Allocates and initializes a block
int insertDirectedLine(Graph *g, vertex u, vertex v, weight value); //Inserts a directed line from vertex u to vertex v
int checkLine(Graph *g, vertex u, vertex v); //Checks if there's a line between vertices u and v
int removeDirectedLine(Graph *g, vertex u, vertex v); // Removes the directed line from vertex u to vertex v
int checkIfThereIsANeighboor(Graph *g, vertex u); //Checks if there's a connection with vertex u
vertex getFirstNeighboor(Graph *g, vertex u); //Returns one vertex that is connected with the vertex u

//Utils
void printGraph(Graph *g); //Shows a representation of a graph
void printBlock(block *u); //Shows a representation of a block

#endif
