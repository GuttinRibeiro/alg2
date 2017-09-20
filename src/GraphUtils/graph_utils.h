#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

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
