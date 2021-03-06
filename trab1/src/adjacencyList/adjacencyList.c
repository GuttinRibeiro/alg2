#include <stdio.h>
#include <stdlib.h>
#include "adjacencyList.h"

void printGraph(Graph *g) {
  printf("Number of vertex: %d\n", g->numVertex);
  printf("Graph representation: adjacency list\n");
  int i;
  for(i = 0; i < g->numVertex; i++) {
    block *curr = g->list[i];
    printf("Line %d: ", i);
    while(curr != NULL) {
      printf("%f ", curr->value);
      curr = curr->next;
    }
    printf("\n");
  }
  printf("\n");
}

void printBlock(block *u) {
  printf("block: %d\n", u->id);
  printf("value: %.2f\n", u->value);

  if(u->next != NULL) {
    printf("next: %d\n", u->next->id);
  }
  else {
    printf("next = NULL\n");
  }
}

/*Graph's functions*/

/* [INTERNAL] Initializes a square matrix size x size
 * Return: void
*/
void _initSquareMatrix(weight **matrix, int size) {
  int i, j;
  for(i = 0; i < size; i++) {
    for(j = 0; j < size; j++) {
      matrix[i][j] = INF;
    }
  }
}

int initGraph(Graph *g, int numVertex) {
  if(numVertex > MAXNUMVERTICES) {
    return -1;
  }

  g->numVertex = numVertex;
  int i;
  for(i = 0; i < g->numVertex; i++) {
    g->list[i] = NULL;
  }

  return 1;
}

int finishGraph(Graph *g) {
  if(g == NULL) {
    return -1;
  }

  block *curr;
  int i;
  for(i = 0; i < g->numVertex; i++) {
      curr = g->list[i];
      while(curr != NULL) {
        g->list[i] = g->list[i]->next;
        free(curr);
        curr = g->list[i];
      }
  }
  return 1;
}

/* [INTERNAL] Initializes a block with an id u and a weight value.
 * Return: [block*]
*/
block* _initBlock(vertex u, weight value) {
  block *newBlock = (block *)malloc(sizeof(block));
  if(newBlock == NULL) {
    return NULL;
  }

  newBlock->id = u;
  newBlock->value = value;
  return newBlock;
}

int insertDirectedLine(Graph *g, vertex u, vertex v, weight value) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  block *newV = _initBlock(v, value);
  if(newV == NULL) {
    return 0;
  }

  /*Adding a line between u and v*/
  block *aux = g->list[u];
  g->list[u] = newV;
  g->list[u]->next = aux;

  return 1;
}

int checkDirectedLine(Graph *g, vertex u, vertex v) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  block *curr = g->list[u];
  while(curr != NULL) {
    if(curr->id == v) {
      return 1;
    }
    curr = curr->next;
  }

  return 0;
}

int removeDirectedLine(Graph *g, vertex u, vertex v) {
  block *curr = g->list[u];
  block *prev;
  while(curr != NULL && curr->id != v) {
    prev = curr;
    curr = curr->next;
  }

  if(curr == NULL) {
    return 0;
  }

  /* if the line that should be removed is the first one*/
  if(curr == g->list[u]) {
    g->list[u] = curr->next;
  } else {
    prev->next = curr->next;
  }

  free(curr);
  return 1;
}

int checkIfThereIsANeighboor(Graph *g, vertex u) {
  if(u >= g->numVertex) {
    return -1;
  }

  if(g->list[u] == NULL) {
    return 0;
  }

  return 1;
}

void getMatrix(Graph *g, weight **matrix) {
  _initSquareMatrix(matrix, g->numVertex);

  /*O(V+A) => to create a copy of an adjacency list*/
  block *curr;
  int i;
  for(i = 0; i < g->numVertex; i++) {
    curr = g->list[i];
    while(curr != NULL) {
      matrix[i][curr->id] = curr->value;
      curr = curr->next;
    }
  }
}

vertex getFirstNeighboor(Graph *g, vertex u) {
  if(u >= g->numVertex || g->list[u] == NULL) {
    return -1;
  }

  return (g->list[u]->id);
}

weight getWeight(Graph *g, vertex u, vertex v) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  block *curr = g->list[u];
  while(curr != NULL && curr->id != v) {
    curr = curr->next;
  }

  if(curr == NULL) {
    return INF;
  }

  return curr->value;
}
