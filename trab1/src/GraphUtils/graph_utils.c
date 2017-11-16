#include "graph_utils.h"

/* _FWInit is an internal function that prepares
 * the output matrix for the FloydWarshall.
 * Basicaly, this function transform the graph
 * into a matrix representation of it
 */
void _FWInit(Graph *g, weight **output) {
  getMatrix(g, output);

  int i;
  for(i = 0; i < g->numVertex; i++) {
    output[i][i] = 0;
  }
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

void FloydWarshallPath(Graph *g, weight **output, List **path) {
  /* Tranform g in a matrix of weights, where
   * the weight to itself is 0
   */
  _FWInit(g, output);

  int i;
  for(i = 0; i < g->numVertex; i++) {
   int j;
   for(j = 0; j < g->numVertex; j++) {
     listInit(&(path[i][j]));
   }
  }

  int k;
  for(k = 0; k < g->numVertex; k++) {
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

        weight aux = output[i][k] + output[k][j];

        if(aux <= output[i][j] && aux < INF) {
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

/* _GraphPathCounter is an internal and recursive
 * function that counts the number of minimum paths
 * from an initial vertex (inside *p) to an current
 * vertex, also, this functions counts the number
 * of such minumum paths that pass through a
 * vertex k
 */
void _GraphPathCounter(struct _PathCounterStructStaticInfo *p, vertex curr, int isPathWithK) {
  if(curr == p->initial) {
    return;
  }

  if(p->path[p->initial][curr].size > 0) {
    *(p->npath) += p->path[p->initial][curr].size - 1;
  }

  if(isPathWithK) {
    if(p->path[p->initial][curr].size > 0) {
      *(p->npathWithK) += p->path[p->initial][curr].size -1;
    }
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

void GraphEccentricityFW(Graph *g, weight **FWoutput, weight *output) {
  int j;
  for(j = 0; j < g->numVertex; j++) {
    output[j] = NINF;

    int i;
    for(i = 0; i < g->numVertex; i++) {
      if(FWoutput[i][j] > output[j] && FWoutput[i][j] <= INF) {
        output[j] = FWoutput[i][j];
      }
    }

  }
}

int GraphEccentricity(Graph *g, weight *output) {
  weight **fwoutput = (weight **)newMatrix(g->numVertex, g->numVertex, sizeof(weight));

  if(fwoutput == NULL) {
    return -1;
  }

  FloydWarshall(g, fwoutput);
  GraphEccentricityFW(g, fwoutput, output);

  deleteMatrix((void **)fwoutput, g->numVertex);
  return 0;
}

int GraphCentralityFW(Graph *g, weight **FWoutput, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  GraphEccentricityFW(g, FWoutput, output);
  GraphCentralityEC(g, output, central);

  free(output);
  return 0;
}

void GraphCentralityEC(Graph *g, weight *ECoutput, vertex *central) {
  weight min = INF;

  int i;
  for(i = 0; i < g->numVertex; i++) {
    if(ECoutput[i] < min) {
      min = ECoutput[i];
      *central = i;
    }
  }
}

int GraphCentrality(Graph *g, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  if(GraphEccentricity(g, output) != 0) {
    free(output);
    return -2;
  }

  GraphCentralityEC(g, output, central);

  free(output);
  return 0;
}

void GraphBetweenessCentralityFWP(Graph *g, float *output, List **path) {
  int k;
  for(k = 0; k < g->numVertex; k++) {
    int npath = 0;
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

        if(path[i][j].size > 0) {
          npath++;
        }
        GraphPathCounter(path, i, j, k, &npath, &npathWithK);
      }
    }

    if(npath == 0) {
      output[k] = 0;
    } else {
      output[k] = (float)npathWithK/npath;
    }
  }
}

int GraphBetweenessCentrality(Graph *g, weight *output) {
  weight **fwoutput = (weight **)newMatrix(g->numVertex, g->numVertex, sizeof(weight));

  if(fwoutput == NULL) {
    return -1;
  }

  List **path = (List **)newMatrix(g->numVertex, g->numVertex, sizeof(List));

  if(path == NULL) {
    deleteMatrix((void **)fwoutput, g->numVertex);
    return -1;
  }

  int i;
  for(i = 0; i < g->numVertex; i++) {
    int j;
    for(j = 0; j < g->numVertex; j++) {
      listInit(&(path[i][j]));
    }
  }

  FloydWarshallPath(g, fwoutput, path);
  GraphBetweenessCentralityFWP(g, output, path);

  deleteMatrix((void **)fwoutput, g->numVertex);

  for(i = 0; i < g->numVertex; i++) {
    int j;
    for(j = 0; j < g->numVertex; j++) {
      listClean(&(path[i][j]));
    }
  }
  deleteMatrix((void **)path, g->numVertex);

  return 0;
}

int GraphCentralBetweeness(Graph *g, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  if(GraphBetweenessCentrality(g, output) < 0) {
    free(output);
    return -2;
  }

  GraphCentralBetweenessBC(g, output, central);

  free(output);
  return 0;


}

void GraphCentralBetweenessBC(Graph *g, weight *BCoutput, vertex *central) {
  weight max = NINF;

  int i;
  for(i = 0; i < g->numVertex; i++) {
    if(BCoutput[i] > max) {
      max = BCoutput[i];
      *central = i;
    }
  }
}

int GraphCentralBetweenessFWP(Graph *g, List **path, vertex *central) {
  weight *output = (weight *)malloc(g->numVertex*sizeof(weight));
  if(output == NULL) {
    return -1;
  }

  GraphBetweenessCentralityFWP(g, output, path);
  GraphCentralBetweenessBC(g, output, central);

  free(output);
  return 0;
}
