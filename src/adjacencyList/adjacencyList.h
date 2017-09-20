#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <float.h>

#define MAXNUMVERTEX 100
#define INF FLT_MAX
#define NINF FLT_MIN

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

//All these functions returns a non positive value when there's something not expected:
/*
  -1 : indicates that one of the parameters has a problem
   0 : indicates that the function hasn't been performed or that it logical value is equal to false
   1 : indicates that the function has been performed or that it logical value is equal to true
*/
//The graph considers that the vertex counting start at zero
int initGraph(Graph *g, int numVertex); //Initializes a graph
int finishGraph(Graph *g); //Finishes a graph
int insertDirectedLine(Graph *g, vertex u, vertex v, weight value); //Inserts a directed line from vertex u to vertex v
int checkDirectedLine(Graph *g, vertex u, vertex v); //Checks if there's a line between vertices u and v
int removeDirectedLine(Graph *g, vertex u, vertex v); // Removes the directed line from vertex u to vertex v
int checkIfThereIsANeighboor(Graph *g, vertex u); //Checks if there's a connection with vertex u
vertex getFirstNeighboor(Graph *g, vertex u); //Returns one vertex that is connected with the vertex u
weight getWeight(Graph *g, vertex u, vertex v);

//Utils
void printGraph(Graph *g); //Shows a representation of a graph
void printBlock(block *u); //Shows a representation of a block

#endif
