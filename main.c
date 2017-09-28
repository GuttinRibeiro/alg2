#include <stdio.h>
#include <stdlib.h>
#include "src/adjacencyList/adjacencyList.h"
#include "src/GraphUtils/graph_utils.h"

int main () {
  // int numCities, numRoads;
  // scanf("%d %d", &numCities, &numRoads);
  // printf("numCities: %d numRoads: %d\n", numCities, numRoads);
  //
  // int numStudents[numCities];
  // int i, j;
  // for(i = 0; i < numCities; i++) {
  //   scanf(" %d", &numStudents[i]);
  //   printf("%d\n", numStudents[i]);
  // }
  // printf("\n");
  //
  Graph g;
  // initGraph(&g, numCities);
  initGraph(&g, 5);
  // weight aux;
  // vertex origin, destination;
  // for(i = 0; i < numRoads; i++) {
  //   scanf(" %d %d %f", &origin, &destination, &aux);
  //   printf("origin: %d destination: %d aux: %f\n", origin, destination, aux);
  //   insertDirectedLine(&g, origin, destination, aux);
  // }

  insertDirectedLine(&g, 0, 1, 1);
  insertDirectedLine(&g, 1, 2, 2);
  insertDirectedLine(&g, 2, 3, 2);
  insertDirectedLine(&g, 3, 2, 3);
  insertDirectedLine(&g, 3, 1, 1);
  insertDirectedLine(&g, 2, 4, 4);
  insertDirectedLine(&g, 4, 3, 5);


  weight **matrixFW = _FWCreateMatrix(5);
  if(matrixFW == NULL) {
    return -50;
  }
  getMatrix(&g, matrixFW);


  int i;
  for (i = 0; i < 5; i++) {
    int j;
    for (j = 0; j < 5; j++) {
      if(matrixFW[i][j] == INF) {
        printf("INF ");
      } else {
        printf("%.2f ", matrixFW[i][j]);
      }
    }
    printf("\n");
  }

  printf("\n");

  FloydWarshall(&g, matrixFW);
  for (i = 0; i < 5; i++) {
    int j;
    for (j = 0; j < 5; j++) {
      if(matrixFW[i][j] == INF) {
        printf("INF ");
      } else {
        printf("%.2f ", matrixFW[i][j]);
      }
    }
    printf("\n");
  }
  printf("\n");

  weight *aux = (weight *)malloc(5 * sizeof(weight));
  GraphEccentricity(&g, aux);
  for (i = 0; i < 5; i++) {
    if(aux[i] == INF) {
      printf("INF ");
    } else {
      printf("%.2f ", aux[i]);
    }
  }
  printf("\n");

  vertex central = 1000;
  GraphCentrality(&g, &central);
  printf("%d ", central);
  printf("\n");

  GraphBetweenessCentrality(&g, aux);
  for (i = 0; i < 5; i++) {
    if(aux[i] == INF) {
      printf("INF ");
    } else {
      printf("%f ", aux[i]);
    }
  }
  printf("\n");

  // weight **matrixFW = _FWCreateMatrix(numCities);
  // FloydWarshall(&g, matrixFW);
  //
  // for(i = 0; i < numCities; i++) {
  //   for(j = 0; j < numCities; j++) {
  //     matrixFW[i][j] *= numStudents[j];
  //     printf("%.2f ", matrixFW[i][j]);
  //   }
  //   printf("\n");
  // }
  // printf("\n");
  //
  // vertex central = 1000;
  // printf("%d %d\n", GraphCentralityFW(&g, matrixFW, &central), central);
  //
  // _FWDestroyMatrix(matrixFW, numCities);
  //
  // printGraph(&g);
  return 0;
}
