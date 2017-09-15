#include <stdio.h>
#include <stdlib.h>
#include "adjacencyMatrix.h"

void initGraph(Graph *g, int numVertex) {
  g->numVertex = numVertex;
  for(int i = 0; i < g->numVertex; i++) {
    for(int j = 0; j < g->numVertex; j++) {
      g->matrix[i][j] = 0.0;
    }
  }
}

int insertLine(Graph *g, block u, block v, weight value) {
  if(u > g->numVertex || v > g->numVertex) {
    return 0;
  }

  g->matrix[u][v] = value;
  g->matrix[v][u] = value;
  return 1;
}

int checkLine(Graph *g, block u, block v) {
  if(u > g->numVertex || v > g->numVertex) {
    return 0;
  }

  if(g->matrix[u][v] > 0.0) {
    return 1;
  }

  return 0;
}

int removeLine(Graph *g, block u, block v) {
  if(u > g->numVertex || v > g->numVertex) {
    return 0;
  }

  g->matrix[u][v] = 0.0;
  return 1;
}

int checkIfThereIsANeighboor(Graph *g, block u) {
  if(u > g->numVertex) {
    return -1;
  }

  for(int i = 0; i < g->numVertex; i++) {
    if(g->matrix[u][i] > 0.0) {
      return 1;
    }
  }

  return 0;
}

int getFirstNeighboor(Graph *g, block u) {
  if(u > g->numVertex) {
    return -1;
  }

  for(int i = 0; i < g->numVertex; i++) {
    if(g->matrix[u][i] > 0.0) {
      return i;
    }
  }

  return 0;
}
