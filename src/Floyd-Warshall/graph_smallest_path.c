#include "graph_smallest_path.h"

/** Complexity: O(v^2)
 */
void FWInit(Graph *g, weight **output) {
  int i;
  for(i = 0; i < g->numVertex; i++) {
    int j;
    for(j = 0; j < g->numVertex; j++) {
      output[i][j] = getWeight(g, i, j);
    }
  }
}

/** Complexity: O(v^3)
 */
void FloydeWarshall(Graph *g, weight **output) {
  FWInit(g, output);

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

/** Complexity: O(v)
 */
weight **FWCreateMatrix(int n) {
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

/** Complexity: O(v)
 */
void FWDestroyMatrix(weight **matrix, int n) {
  int i;
  for(i = 0; i < n; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

/** Complexity: O(v)
 */
weight MinWeight(weight *output, int n, int *index) {
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

/** Complexity: O(v)
 */
weight MaxWeight(weight *output, int n) {
  weight max = NINF;

  int i;
  for(i = 0; i < n; i++) {
    if(output[i] > max && output[i] < INF) {
      max = output[i];
    }
  }

  return min;
}

/** Complexity: O(v^3)
 */
int GraphEccentricity(Graph *g, weight *output) {
  weight **output = FWCreateMatrix(g->numVertex);

  if(output == NULL) {
    return -1;
  }

  FloydeWarshall(g, output);

  int i;
  for(i = 0; i < g->numVertex; i++) {
    output[i] = MaxWeight(g[i], g->numVertex, NULL);
  }

  FWDestroyMatrix(output);

  return 0;
}

/** Complexity: O(v^3)
 */
int GraphCentrality(Graph *g, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(weight == NULL) {
    return -1;
  }

  if(GraphEccentricity(g, output) != 0) {
    return -2;
  }

  MinWeight(output, g->numVertex, central);

  free(output);
  return 0;
}
