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
block *initBlock(vertex u, weight value, int *err); //Allocates and initializes a block
int insertLine(Graph *g, vertex u, weight valueU, vertex v, weight valueV); //Insert a line between vertices u and v
int checkLine(Graph *g, vertex u, vertex v); //Checks if there's a line between vertices u and v
// Remove the directed line u->v in the graph (*g)
int _removeDirectedLine(Graph *g, vertex u, vertex v);

int removeLine(Graph *g, vertex u, vertex v); //Removes a line between two vertices
int checkIfThereIsANeighboor(Graph *g, vertex u); //Checks if there's a connection with vertex u
vertex getFirstNeighboor(Graph *g, vertex u); //Returns one vertex that is connected with the vertex u

//Utils
void printGraph(Graph *g); //Shows a representation of graph
void printBlock(block *u); //Shows a representation of block

#endif
