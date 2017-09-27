#include <stdio.h>
#include <stdlib.h>
#include "adjacencyMatrix.h"

//Utils
void printGraph(Graph *g) {
  printf("Number of vertex: %d\n", g->numVertex);
  printf("Graph representation: adjacency matrix\n");
  for(int i = 0; i < g->numVertex; i++) {
    for(int j = 0; j < g->numVertex; j++) {
      printf("%.2f ", g->matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

//Graph's functions
void initSquareMatrix(weight **matrix, int size) {
  if(size > MAXNUMVERTEX) {
    return;
  }

  int i, j;
  for(i = 0; i < size; i++) {
    for(j = 0; j < size; j++) {
      matrix[i][j] = INF;
    }
  }

  return;
}

int initGraph(Graph *g, int numVertex) {
  if(numVertex > MAXNUMVERTEX) {
    return -1;
  }

  g->numVertex = numVertex;
  if(initSquareMatrix(g->matrix, g->numVertex)) {
    return 0;
  }

  return 1;
}

int insertDirectedLine(Graph *g, vertex u, vertex v, weight value) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  g->matrix[u][v] = value;
  return 1;
}

int checkDirectedLine(Graph *g, vertex u, vertex v) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  if(g->matrix[u][v] != INF) {
    return 1;
  }

  return 0;
}

int removeDirectedLine(Graph *g, vertex u, vertex v) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  g->matrix[u][v] = INF;
  return 1;
}

int checkIfThereIsANeighboor(Graph *g, vertex u) {
  if(u >= g->numVertex) {
    return -1;
  }

  for(int i = 0; i < g->numVertex; i++) {
    if(g->matrix[u][i] != INF) {
      return 1;
    }
  }

  return 0;
}

vertex getFirstNeighboor(Graph *g, vertex u) {
  if(u >= g->numVertex) {
    return -1;
  }

  for(int i = 0; i < g->numVertex; i++) {
    if(g->matrix[u][i] != INF) {
      return i;
    }
  }

  return 0;
}

weight getWeight(Graph *g, vertex u, vertex v) {
  return g->matrix[u][v];
}

void getMatrix(Graph *g, weight **matrix) {
  //O(V²) => to create a copy of an adjacency matrix
  initSquareMatrix(matrix, g->numVertex);

  int i, j;
  for(i = 0; i < g->numVertex; i++) {
    for (j = 0; j < g->numVertex; j++) {
        matrix[i][j] = g->matrix[i][j];
    }
  }
}
