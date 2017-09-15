#ifndef ADJACENCYMATRIX_h
#define ADJACENCYMATRIX_h

#define MAXNUMVERTEX 100

typedef int block;
typedef float weight;

typedef struct {
  weight matrix[MAXNUMVERTEX][MAXNUMVERTEX];
  int numVertex;
} Graph;

void initGraph(Graph *g, int numVertex);
int insertLine(Graph *g, block u, block v, weight value);
int checkLine(Graph *g, block u, block v);
int removeLine(Graph *g, block u, block v);
int checkIfThereIsANeighboor(Graph *g, block u);
int getFirstNeighboor(Graph *g, block u);

#endif
