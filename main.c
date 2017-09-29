#include <stdio.h>
#include <stdlib.h>
#include "src/adjacencyList/adjacencyList.h"
#include "src/GraphUtils/graph_utils.h"
#include "src/Utils/utils.h"

int main () {
  int numCities, numRoads;

  printf("Numero de cidades: ");
  scanf(" %d", &numCities);

  printf("Numero de estradas: ");
  scanf(" %d", &numRoads);

  printf("\nDigite o numero de estudantes por cidade: \n");
  int numStudents[numCities];
  int i;
  for(i = 0; i < numCities; i++) {
    printf("Cidade %d: ", i);
    scanf(" %d", &numStudents[i]);
  }
  printf("\n");

  Graph g;
  initGraph(&g, numCities);
  weight aux;
  vertex origin, destination;

  printf("Digite o vertices na ordem [origem] [destino] [peso]:\n");
  for(i = 0; i < numRoads; i++) {
    scanf(" %d %d %f", &origin, &destination, &aux);
    printf("origin: %d destination: %d aux: %f\n", origin, destination, aux);

    if(insertDirectedLine(&g, origin, destination, aux) <= 0) {
      fprintf(stderr, "Entrada invalida.\n");
      i--;
    }
  }

  weight **matrixFW = (weight **)newMatrix(numCities, numCities, sizeof(weight));
  List **path = (List **)newMatrix(numCities, numCities, sizeof(List));
  if(matrixFW == NULL || path == NULL) {
    fprintf(stderr, "[Error] Erro ao alocar matriz auxiliar.\n");
    return -1;
  }

  FloydWarshallPath(&g, matrixFW, path);

  for(i = 0; i < g.numVertex; i++) {
    int j;
    for(j = 0; j < g.numVertex; j++) {
      matrixFW[i][j] = numStudents[i];
    }
  }

  vertex central = 1000;
  GraphCentralityFW(&g, matrixFW, &central);

  vertex betweness = 1000;
  GraphCentralBetweenessFWP(&g, path, &betweness);

  printf("Saida: %d, %d\n", central, betweness);

  deleteMatrix((void **)matrixFW, numCities);
  deleteMatrix((void **)path, numCities);

  finishGraph(&g);

  return 0;
}
