#include "graph_utils.h"

/* _FWInit is an internal function that prepares
 * the output matrix for the FloydWarshall.
 * Basicaly, this function transform the graph
 * into a matrix representation of it
 * Complexity: O(v^2)*O(getWeight)
 */
void _FWInit(Graph *g, weight **output) {
  int i;
  for(i = 0; i < g->numVertex; i++) {
    int j;
    for(j = 0; j < g->numVertex; j++) {
      if(i == j) {
        output[i][j] = 0.0;
      } else {
        output[i][j] = getWeight(g, i, j);
      }
    }
  }
}

// external function
void FloydWarshall(Graph *g, weight **output) {
  _FWInit(g, output);

  int k;
  for(k = 0; k < g->numVertex; k++) {
    int i;
    for(i = 0; i < g->numVertex; i++) {
      int j;
      for(j = 0; j < g->numVertex; j++) {
        weight aux = output[i][k] + output[k][j];

        if(aux < output[i][j]) {
          output[i][j] = aux;
        }
      }
    }
  }
}

/* _FWCreateMatrix is an internal function that
 * creates a matrix nxn.
 */
weight **_FWCreateMatrix(int n) {
  weight **matrix = (weight **)malloc(n*sizeof(weight *));
  if(matrix == NULL) {
    return NULL;
  }

  int i;
  for(i = 0; i < n; i++) {
    matrix[i] = (weight *)malloc(n*sizeof(weight));
    if(matrix[i] == NULL) {
      while(--i >= 0) {
        free(matrix[i]);
      }
      free(matrix);

      return NULL;
    }
  }

  return matrix;
}

/* _FWDestroyMatrix is an internal function that
 * deallocate the matrix created in _FWCreateMatrix
 */
void _FWDestroyMatrix(weight **matrix, int n) {
  int i;
  for(i = 0; i < n; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

/* _MinWeight is an internal function that
 * returns the minimum weight in a vector
 * and the index of such.
 */
weight _MinWeight(weight *output, int n, vertex *index) {
  weight min = INF;

  int i;
  for(i = 0; i < n; i++) {
    if(output[i] < min) {
      min = output[i];
      *index = i;
    }
  }

  return min;
}

/* _MaxWeight is an internal function that
 * return the maximum weight in a vector.
 */
weight _MaxWeightColumn(weight **output, vertex column, int n) {
  weight max = NINF;

  int i;
  for(i = 0; i < n; i++) {
    if(output[i][column] > max && output[i][column] <= INF) {
      max = output[i][column];
    }
  }

  return max;
}

int GraphEccentricity(Graph *g, weight *output) {
  weight **fwoutput = _FWCreateMatrix(g->numVertex);

  if(fwoutput == NULL) {
    return -1;
  }

  FloydWarshall(g, fwoutput);

  int i;
  for(i = 0; i < g->numVertex; i++) {
    int j;
    for(j = 0; j < g->numVertex; j++) {
      printf("%.2f ", fwoutput[i][j]);
    }
    printf("\n");
  }

  for(i = 0; i < g->numVertex; i++) {
    output[i] = _MaxWeightColumn(fwoutput, i, g->numVertex);
  }

  _FWDestroyMatrix(fwoutput, g->numVertex);

  return 0;
}

int GraphCentrality(Graph *g, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  if(GraphEccentricity(g, output) != 0) {
    return -2;
  }

  _MinWeight(output, g->numVertex, central);

  free(output);
  return 0;
}
