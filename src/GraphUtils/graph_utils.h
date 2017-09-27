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

/* GraphEccentricityFW will calculate the eccentricity
 * of ALL vertex using the output of a previous execution
 * of the Floyd-Warshall Algorithm (FWoutput).
 * The output will be a vector 1xv, where v is the
 * number of vertex in the graph g
 *
 * Arguments: Graph *g            The graph to be use in the algorithm
 *            weight **FWoutput   The output of FloydWarshall
 *            weight *output      The vector that will hold the result
 * Return: (void)
 * Complexity: O(V²)
 */
void GraphEccentricityFW(Graph *g, weight **FWoutput, weight *output);

/* GraphCentrality will calculate the central vertex
 * using the eccentricity.
 * The output will be a vertex (*central)
 *
 * Arguments: Graph *g          The graph to be use in the algorithm
 *            vertex *central   The central vertex (output)
 * Return: int  [-1] A error occur when allocating memory
                [-2] A error occur when calculating eccentricity
 *              [0]  No error
 * Complexity: O(V³)
 */
int GraphCentrality(Graph *g, vertex *central);

/* GraphCentralityFW will calculate the central vertex
 * using the output of a previous execution of the
 * Floyd-Warshall Algorithm (FWoutput).
 * The output will be a vertex (*central)
 *
 * Arguments: Graph *g          The graph to be use in the algorithm
 *            weight **FWoutput   The output of FloydWarshall
 *            vertex *central   The central vertex (output)
 * Return: int  [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(V²)
 */
int GraphCentralityFW(Graph *g, weight **FWoutput, vertex *central);

/* GraphCentralityEC will calculate the central vertex
 * using the output of a previous execution of the
 * GraphEccentricity (ECoutput).
 * The output will be a vertex (*central)
 *
 * Arguments: Graph *g          The graph to be use in the algorithm
 *            weight *ECoutput   The output of GraphEccentricity
 *            vertex *central   The central vertex (output)
 * Return: int  [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(V)
 */
void GraphCentralityEC(Graph *g, weight *ECoutput, vertex *central);

/* GraphBetweenessCentrality will calculate the
 * betweeness centrality using Floyd-Warshall
 * Algorithm.
 * The output will be a vector 1xv, where v is the
 * number of vertex in the graph g
 *
 * Arguments: Graph *g        The graph to be use in the algorithm
 *            weight *output  The vector that will hold the result
 * Return: int  [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(V³) - worst-case O(V⁴)
 */
int GraphBetweenessCentrality(Graph *g, weight *output);

/* GraphBetweenessCentralityFWP will calculate the
 * betweeness centrality using the output of a
 * previous Floyd-Warshall Algorithm (path).
 * The output will be a vector 1xv, where v is the
 * number of vertex in the graph g
 *
 * Arguments: Graph *g        The graph to be use in the algorithm
 *            weight *output  The vector that will hold the result
 *            List **path     The path output of FloydeWarshallPath
 * Return: (void)
 * Complexity: O(V³) - worst-case O(V⁴)
 */
void GraphBetweenessCentralityFWP(Graph *g, weight *output, List **path);

/* GraphCentralBetweeness will calculate the
 * central vertex using the Betweeness Centrality.
 * The output will be a vertex (*central)
 *
 * Arguments: Graph *g          The graph to be use in the algorithm
 *            vertex *central   The central vertex (output)
 * Return: int  [-1] A error occur when allocating memory
                [-2] A error occur when calculating the betweeness centrality
 *              [0]  No error
 * Complexity: O(V³) - worst-case O(V⁴)
 */
int GraphCentralBetweeness(Graph *g, vertex *central);

/* GraphCentralBetweenessBC will calculate the
 * central vertex using a previous output of
 * Betweeness Centrality algorithm.
 * The output will be a vertex (*central)
 *
 * Arguments: Graph *g          The graph to be use in the algorithm
 *            weight *BCoutput  The output of GraphBetweenessCentrality
 *            vertex *central   The central vertex (output)
 * Return: (void)
 * Complexity: O(V)
 */
void GraphCentralBetweenessBC(Graph *g, weight *BCoutput, vertex *central);

/* GraphCentralBetweenessFWP will calculate the
 * central vertex using the Betweeness Centrality
 * and a previous output of Betweeness Centrality
 * algorithm.
 * The output will be a vertex (*central)
 *
 * Arguments: Graph *g          The graph to be use in the algorithm
 *            List **path     The path output of FloydeWarshallPath
 *            vertex *central   The central vertex (output)
 * Return: int  [-1] A error occur when allocating memory
 *              [0]  No error
 * Complexity: O(V³) - worst-case O(V⁴)
 */
 int GraphCentralBetweenessFWP(Graph *g, List **path, vertex *central);

#endif
