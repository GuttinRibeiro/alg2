#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <float.h>

#define MAXNUMVERTICES 50
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
  block *list[MAXNUMVERTICES];
  int numVertex;
} Graph;
//The graph considers that the vertex counting start at zero.

/* Initializes a graph.
 * Return: int [-1] if numVertex is bigger than MAXNUMVERTICES
 *             [1] everything is in order                    */
int initGraph(Graph *g, int numVertex);

/* Finishes a graph.
 * Return: int [-1] if the graph is invalid
 *             [1] everything is in order                    */
int finishGraph(Graph *g);

/* Checks if there's a line from vertex u to vertex v.
 * Return: int [-1] one of the parameters is invalid
 *              [0] there isn't any memory left to create a new block
 *              [1] everything is correct                    */
int insertDirectedLine(Graph *g, vertex u, vertex v, weight value);

/* Inserts a directed line from vertex u to vertex v.
 * Return: int [-1] one of the parameters is invalid
 *              [0] there isn't a line from vertex u to vertex v
 *              [1] there is a line from vertex u to vertex v                    */
int checkDirectedLine(Graph *g, vertex u, vertex v);

/* Removes the directed line from vertex u to vertex v.
 * Return: int  [0] there isn't a line from vertex u to vertex v
 *              [1] line removed   */
int removeDirectedLine(Graph *g, vertex u, vertex v);

/* Checks if there's a connection with vertex u.
 * Return: int  [-1] vertex u is an invalid vertex
 *              [0] vertex u hasn't any neighboor
 *              [1] vertex u has a neighboor                 */
int checkIfThereIsANeighboor(Graph *g, vertex u);

/* Returns a copy of these representation as an adjacency matrix.
 * Return: void
*/
void getMatrix(Graph *g, weight **matrix);

/* Returns one vertex that is connected with the vertex u.
 * Return: int  [-1] vertex u doesn't have any neighboor
 *              [id] the id of first vertex that is a neighboor of vertex u   */
vertex getFirstNeighboor(Graph *g, vertex u);

/* Returns the value of the line from vertex u to vertex v.
 * Return: int  [-1] one of the parameters is invalid
 *              [INF] there isn't a connection from vertex u to vertex v
 *              [weight] the value of the connection from vertex u to vertex v     */
weight getWeight(Graph *g, vertex u, vertex v); //Returns the value of the line from vertex u to vertex v

//Utils
void printGraph(Graph *g); //Shows a representation of the graph
void printBlock(block *u); //Shows a representation of a block

#endif
