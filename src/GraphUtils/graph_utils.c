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

List **_FWCreatePathMatrix(int n) {
  List **matrix = (List **)malloc(n*sizeof(List *));
  if(matrix == NULL) {
    return NULL;
  }

  int i;
  for(i = 0; i < n; i++) {
    matrix[i] = (List *)malloc(n*sizeof(List));
    if(matrix[i] == NULL) {
      while(--i >= 0) {
        free(matrix[i]);
      }
      free(matrix);

      return NULL;
    }

    int j;
    for(j = 0; j < n; j++) {
      listInit(&(matrix[i][j]));
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

void _FWDestroyPathMatrix(List **matrix, int n) {
  int i;
  for(i = 0; i < n; i++) {
    int j;
    for(j = 0; j < n; j++) {
      listClean(&(matrix[i][j]));
    }

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

void FloydWarshall(Graph *g, weight **output) {
  /* Tranform g in a matrix of weights, where
   * the weight to itself is 0
   */
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

void _FWPInit(Graph *g, weight **output, List **path) {
  _FWInit(g, output);

  int i;
  for(i = 0; i < g->numVertex; i++) {
    int j;
    for(j = 0; j < g->numVertex; j++) {
      listPushFront(&(path[i][j]), i);
    }
  }
}

void FloydWarshallPath(Graph *g, weight **output, List **path) {
  /* Tranform g in a matrix of weights, where
   * the weight to itself is 0
   */

  int k;
  for(k = 0; k < g->numVertex; k++) {
    int i;
    for(i = 0; i < g->numVertex; i++) {
      int j;
      for(j = 0; j < g->numVertex; j++) {
        weight aux = output[i][k] + output[k][j];

        if(aux <= output[i][j]) {
          if(aux < output[i][j]) {
            listClean(&(path[i][j]));
          }
          listPushFront(&(path[i][j]), k);

          output[i][j] = aux;
        }
      }
    }
  }
}

void _GraphPathCounter(struct _PathCounterStructStaticInfo *p, vertex curr, int isPathWithK) {
  if(curr == p->initial) {
    return;
  }

  *(p->npath) += p->path[p->initial][curr].size - 1;

  if(isPathWithK) {
    *(p->npathWithK) += p->path[p->initial][curr].size -1;
  } else if(curr == p->k) {
    isPathWithK = 1;
    *(p->npathWithK) += 1;
  }

  listIterator it;
  for(it = itrBegin(&(p->path[p->initial][curr])); it != itrEnd(); itrNext(&it)) {
    _GraphPathCounter(p, itrValue(it), isPathWithK);
  }
}

void GraphPathCounter(List **path, vertex initial, vertex dest, vertex k, int *npath, int *npathWithK) {
  struct _PathCounterStructStaticInfo p;
  p.path = path;
  p.initial = initial;
  p.k = k;
  p.npath = npath;
  p.npathWithK = npathWithK;

  _GraphPathCounter(&p, dest, 0);
}

void GraphBetweenessCentralityFWP(Graph *g, weight *output, List **path) {
  int k;
  for(k = 0; k < g->numVertex; k++) {
    int npath = 1;
    int npathWithK = 0;

    int i;
    for(i = 0; i < g->numVertex; i++) {
      if(i == k) {
        continue;
      }

      int j;
      for(j = 0; j < g->numVertex; j++) {
        if(j == k) {
          continue;
        }

        GraphPathCounter(path, i, j, k, &npath, &npathWithK);
      }
    }

    output[k] = npathWithK/npath;
  }
}

int GraphBetweenessCentrality(Graph *g, weight *output) {
  weight **fwoutput = _FWCreateMatrix(g->numVertex);

  if(fwoutput == NULL) {
    return -1;
  }

  List **path = (List **)_FWCreatePathMatrix(g->numVertex);

  if(path == NULL) {
    _FWDestroyMatrix(fwoutput, g->numVertex);
    return -1;
  }

  FloydWarshallPath(g, fwoutput, path);

  GraphBetweenessCentralityFWP(g, output, path);

  _FWDestroyMatrix(fwoutput, g->numVertex);
  _FWDestroyPathMatrix(path, g->numVertex);

  return 0;
}

void GraphEccentricityFW(Graph *g, weight **FWoutput, weight *output) {
  int i;
  for(i = 0; i < g->numVertex; i++) {
    output[i] = _MaxWeightColumn(FWoutput, i, g->numVertex);
  }
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

  GraphEccentricityFW(g, fwoutput, output);

  _FWDestroyMatrix(fwoutput, g->numVertex);

  return 0;
}

int GraphCentralityFW(Graph *g, weight **FWoutput, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  GraphEccentricityFW(g, FWoutput, output);

  _MinWeight(output, g->numVertex, central);

  free(output);
  return 0;
}

void GraphCentralityEC(Graph *g, weight *ECoutput, vertex *central) {
  _MinWeight(ECoutput, g->numVertex, central);
}

int GraphCentrality(Graph *g, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  if(GraphEccentricity(g, output) != 0) {
    return -2;
  }

  GraphCentralityEC(g, output, central);

  free(output);
  return 0;
}
