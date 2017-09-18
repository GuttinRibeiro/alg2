#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#define MAXNUMVERTEX 100

typedef struct node {
  int vertex;
  float weight;
  struct node *next;
} block;

typedef struct {
  block *list[MAXNUMVERTEX];
  int numVertex;
} Graph;

//All these functions returns a negative value when there's an error
//The graph considers that the vertex counting start at zero.
int initGraph(Graph *g, int numVertex); //Initializes a graph
int initBlock(block *u, float weight, int vertex); //Allocates and initializes a block a block
int insertLine(Graph *g, block *u, block *v); //Insert a line
int checkLine(Graph *g, block *u, block *v); //Checks if there's a line between vertex u and v
int removeLine(Graph *g, block *u, block *v); //Removes a line between two vertex
int checkIfThereIsANeighboor(Graph *g, block *u); //Checks if there's a connection with vertex u
block *getFirstNeighboor(Graph *g, block *u); //Returns one vertex that is connected with the vertex u

//Utils
int swapBlocks(block *u, block *v); //Swap blocks u and v
void printGraph(Graph *g); //Shows a representation of graph
void printBlock(block *u);

#endif
