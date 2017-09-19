#include <stdio.h>
#include <stdlib.h>
#include "adjacencyList.h"

//Utils
void printGraph(Graph *g) {
  printf("Number of vertex: %d\n", g->numVertex);
  printf("Graph representation: adjacency list\n");

  for(int i = 0; i < g->numVertex; i++) {
    block *b = g->list[i];
    printf("Line %d: ", i);
    while(b != NULL) {
      printf("%d ", b->id);
      b = b->next;
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

int finishGraph(Graph *g) {
  if(g == NULL) {
    return 0;
  }

  block *p;
  for(int i = 0; i < g->numVertex; i++) {
      p = g->list[i];
      while(p != NULL) {
        g->list[i] = g->list[i]->next;
        free(p);
        p = g->list[i];
      }
  }
  return 1;
}

block* initBlock(vertex u, weight value, int *err) {
  block *newBlock = (block *)malloc(sizeof(block));
  if(newBlock == NULL) {
    *err = 1;
    return NULL;
  }

  newBlock->id = u;
  newBlock->value = value;
  *err = 0;
  return newBlock;
}

int insertLine(Graph *g, vertex u, weight valueU, vertex v, weight valueV) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  int err = 0;
  block *newU = initBlock(u, valueU, &err);
  if(err) {
    return 0;
  }

  block *newV = initBlock(v, valueV, &err);
  if(err) {
    return 0;
  }

  //Line between u and v
  block *aux = g->list[newU->id];
  g->list[newU->id] = newV;
  g->list[newU->id]->next = aux;

  //Line between v and u
  aux = g->list[newV->id];
  g->list[newV->id] = newU;
  g->list[newV->id]->next = aux;

  return 1;
}

int checkLine(Graph *g, vertex u, vertex v) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  block *b = g->list[u];
  while(b != NULL) {
    if(b->id == v) {
      return 1;
    }
    b = b->next;
  }

  return 0;
}

int _removeDirectedLine(Graph *g, vertex u, vertex v) {
  block *curr = g->list[u];
  block *prev;
  while(curr != NULL && curr->id != v) {
    prev = curr;
    curr = curr->next;
  }

  if(curr == NULL) {
    return -1;
  }

  // if the line to be removed is the first one...
  if(curr == g->list[u]) {
    g->list[u] = curr->next;
  } else {
    prev->next = curr->next;
  }

  free(curr);
  return 1;
}

int removeLine(Graph *g, vertex u, vertex v) {
  if(u >= g->numVertex || v >= g->numVertex) {
    return -1;
  }

  return _removeDirectedLine(g,u,v) | _removeDirectedLine(g,v,u);
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

vertex getFirstNeighboor(Graph *g, vertex u) {
  if(u >= g->numVertex || g->list[u] == NULL) {
    return -1;
  }

  return (g->list[u]->id);
}
