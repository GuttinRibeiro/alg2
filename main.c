#include <stdio.h>
#include <stdlib.h>
#include "src/adjacencyList/adjacencyList.h"
#include "src/GraphUtils/graph_utils.h"

int main () {
  int numCities, numRoads;
  scanf("%d %d", &numCities, &numRoads);
  printf("numCities: %d numRoads: %d\n", numCities, numRoads);

  int numStudents[numCities];
  int i, j;
  for(i = 0; i < numCities; i++) {
    scanf(" %d", &numStudents[i]);
    printf("%d\n", numStudents[i]);
  }
  printf("\n");

  Graph g;
  initGraph(&g, numCities);
  weight aux;
  vertex origin, destination;
  for(i = 0; i < numRoads; i++) {
    scanf(" %d %d %f", &origin, &destination, &aux);
    printf("origin: %d destination: %d aux: %f\n", origin, destination, aux);
    insertDirectedLine(&g, origin, destination, aux);
  }

  weight **matrixFW = _FWCreateMatrix(numCities);
  FloydWarshall(&g, matrixFW);

  for(i = 0; i < numCities; i++) {
    for(j = 0; j < numCities; j++) {
      matrixFW[i][j] *= numStudents[j];
      printf("%.2f ", matrixFW[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  vertex central;
  GraphCentrality(&g, &central);
  printf("%d\n", central);

  _FWDestroyMatrix(matrixFW, numCities);

  //printGraph(&g);
  return 0;
}
