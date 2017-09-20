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
      output[i][j] = getWeight(g, i, j);
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
weight _MaxWeight(weight *output, int n) {
  weight max = NINF;

  int i;
  for(i = 0; i < n; i++) {
    if(output[i] > max && output[i] < INF) {
      max = output[i];
    }
  }

  return min;
}

int GraphEccentricity(Graph *g, weight *output) {
  weight **output = _FWCreateMatrix(g->numVertex);

  if(output == NULL) {
    return -1;
  }

  FloydWarshall(g, output);

  int i;
  for(i = 0; i < g->numVertex; i++) {
    output[i] = _MaxWeight(g[i], g->numVertex, NULL);
  }

  _FWDestroyMatrix(output);

  return 0;
}

int GraphCentrality(Graph *g, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(weight == NULL) {
    return -1;
  }

  if(GraphEccentricity(g, output) != 0) {
    return -2;
  }

  _MinWeight(output, g->numVertex, central);

  free(output);
  return 0;
}
