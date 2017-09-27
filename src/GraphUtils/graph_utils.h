#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include "../List/list.h"

#include "../adjacencyList/adjacencyList.h"

#ifndef INF
#define INF 100000
#warning Please define the infinity value of weight.
#endif

#ifndef NINF
#define NINF -100000
#warning Please define the minimum value of weight.
#endif

/* _PathCounterStructStaticInfo is an auxiliary struct
 * to betweness centrality (_GraphPathCounter function)
 */
struct _PathCounterStructStaticInfo {
  List **path;
  vertex initial;
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
 * Complexity: O(V³)
 */
void FloydWarshall(Graph *g, weight **output);

/* FloydeWarshallPath will apply the Floyd-Warshall
 * Algorithm to solve the smallest path of any two
 * vertex in a graph.
 * The output will be a square matrix vxv, where v
 * is the number of vertex in the graph g, AND a
 * matrix that can be converted to the paths that
 * reaches this mininum distance.
 *
 * Arguments: Graph *g         The graph to be use in the algorithm
 *            weight **output  The matrix that will hold the result
 *            List **path      The matrix that will hold the 'path'
 * Return: (void)
 * Complexity: O(V³)
 */
void FloydWarshallPath(Graph *g, weight **output, List **path);

int GraphBetweenessCentrality(Graph *g, weight *output);
void GraphBetweenessCentralityFWP(Graph *g, weight *output, List **path);

/* GraphEccentricity will calculate the eccentricity
 * of ALL vertex using Floyd-Warshall Algorithm.
 * The output will be a vector 1xv, where v is the
 * number of vertex in the graph g
 *
 * Arguments: Graph *g        The graph to be use in the algorithm
 *            weight *output  The vector that will hold the result
 * Return: int  [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(V³)
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
 * Complexity: O(V³)
 */

void GraphEccentricityFW(Graph *g, weight **FWoutput, weight *output);
int GraphCentrality(Graph *g, vertex *central);
int GraphCentralityFW(Graph *g, weight **FWoutput, vertex *central);
void GraphCentralityEC(Graph *g, weight *ECoutput, vertex *central);

#endif
