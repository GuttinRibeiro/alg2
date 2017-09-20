#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include "../List/list.h"

#ifdef ADJACENCYLIST_GRAPH
#include "../adjacencyList/adjacencyList.h"
#else
#include "../adjacencyMatrix/adjacencyMatrix.h"
#endif

#ifndef INF
#define INF 100000
#warning Please define the infinity value of weight.
#endif

#ifndef NINF
#define NINF -100000
#warning Please define the minimum value of weight.
#endif

struct _PathCounterStructStaticInfo {
  List **path;
  vertex dest;
  vertex k;
  int *npath;
  int *npathWithK;
} PathCounter;

/* FloydeWarshall will apply the Floyd-Warshall
 * Algorithm to solve the smallest path of any two
 * vertex in a graph.
 * The output will be a square matrix vxv, where v
 * is the number of vertex in the graph g.
 *
 * Arguments: Graph *g         The graph to be use in the algorithm
 *            weight **output  The matrix that will hold the result
 * Return: (void)
 * Complexity: O(v^3)
 */
void FloydWarshall(Graph *g, weight **output);

/* GraphEccentricity will calculate the eccentricity
 * of ALL vertex using Floyd-Warshall Algorithm.
 * The output will be a vector 1xv, where v is the
 * number of vertex in the graph g
 *
 * Arguments: Graph *g        The graph to be use in the algorithm
 *            weight *output  The vector that will hold the result
 * Return: int  [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(v^3)
 */
int GraphEccentricity(Graph *g, weight *output);

/* GraphCentrality will calculate the most central
 * vertex in the Graph using eccentricity.
 * The output will be a vertex *central
 *
 * Arguments: Graph *g         The graph to be use in the algorithm
 *            vertex *central  The variable that will hold the result
 * Return: int  [-2] A error occur when calculating the eccentricity
 *              [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(v^3)
 */
int GraphCentrality(Graph *g, vertex *central);

#endif
