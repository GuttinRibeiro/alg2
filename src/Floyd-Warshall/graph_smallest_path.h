#ifndef GRAPH_SMALLEST_PATH_H
#define GRAPH_SMALLEST_PATH_H

#ifndef Graph
#error Unknown definition of Graph.
#endif

#ifndef weight
#error Unknown definition of weight.
#endif

#ifndef vertex
#error Unknown definition of vertex.
#endif

#ifndef INF
#define INF 10000
#warning Please define the infinity value of weight.
#endif

#ifndef NINF
#define NINF -10000
#warning Please define the minimum value of weight.
#endif

void FloydeWarshall(Graph *g, weight **output);
int GraphEccentricity(Graph *g, weight *output);
int GraphCentrality(Graph *g, vertex *central);

#endif
