#include <stdio.h>
#include <stdlib.h>
#include "adjacencyList.h"

//Utils
int swapBlocks(block *u, block *v) {
  if(u == NULL || v == NULL) {
    return -1;
  }

  block *aux = u;
  u = v;
  v = aux;

  return 1;
}

void printGraph(Graph *g) {
  printf("Number of vertex: %d\n", g->numVertex);
  printf("Graph representation: adjacency list\n");

  for(int i = 0; i < g->numVertex; i++) {
    block *b = g->list[i];
    printf("Line %d: ", i);
    while(b != NULL) {
      printf("%d ", b->vertex);
      b = b->next;
    }
    printf("\n");
  }
  printf("\n");
}

void printBlock(block *u) {
  printf("block: %d\n", u->vertex);
  printf("value: %.2f\n", u->weight);
}
//Graph's functions
int initGraph(Graph *g, int numVertex) {
  if(numVertex > MAXNUMVERTEX) {
    return -1;
  }

  g->numVertex = numVertex;
  for(int i = 0; i < g->numVertex; i++) {
    g->list[i] = NULL;
  }

  return 1;
}

int initBlock(block *u, float weight, int vertex) {
  if(u == NULL) {
    return -1;
  }

  u->weight = weight;
  u->vertex = vertex;
  u->next = NULL;

  return 1;
}

int insertLine(Graph *g, block *u, block *v) {
  if(u == NULL || v == NULL) {
    return -1;
  }

  //Line between u and v
  block *aux = g->list[u->vertex];
  g->list[u->vertex] = v;
  printGraph(g);
  g->list[u->vertex]->next = aux;
  printGraph(g);
  //Line between v and u
  aux = g->list[v->vertex];
  g->list[v->vertex] = u;
  printGraph(g);
  g->list[v->vertex]->next = aux;
  printGraph(g);

  return 1;
}

int checkLine(Graph *g, block *u, block *v) {
  if(u == NULL || v == NULL) {
    return -1;
  }

  int swap = 0;
  //Get smaller id for block u
  if(u->vertex > v->vertex) {
    swapBlocks(u, v);
    swap = 1;
  }

  block *b = g->list[u->vertex];
  while(b != NULL) {
    if(b->vertex == v->vertex) {
      if(swap) {
        swapBlocks(u, v);
      }
      return 1;
    }
    b = b->next;
  }

  return 0;
}

int removeLine(Graph *g, block *u, block *v) {
  if(u == NULL || v == NULL) {
    return -1;
  }

  int swap = 0;
  //Get smaller id for block u
  if(u->vertex > v->vertex) {
    swapBlocks(u, v);
    swap = 1;
  }

  block *b1 = g->list[u->vertex];
  while(b1 != NULL) {
    if(b1->next->vertex == v->vertex) {
      block *p1 = b1;
      p1 = p1->next;
      b1->next = p1->next;

      block *b2 = g->list[v->vertex];
      while(b2->next->vertex != u->vertex) {
        b2 = b2->next;
      }

      block *p2 = b2;
      p2 = p2->next;
      b2->next = p2->next;

      free(p1);
      free(p2);

      return 1;
    }
    b1 = b1->next;
  }

  return 0;
}

int checkIfThereIsANeighboor(Graph *g, block *u) {
  if(u == NULL) {
    return -1;
  }

  if(g->list[u->vertex] == NULL) {
    return 0;
  }

  return 1;
}

block *getFirstNeighboor(Graph *g, block *u) {
  if(u == NULL) {
    return NULL;
  }

  return (g->list[u->vertex]->next);
}
