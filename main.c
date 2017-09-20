#include <stdio.h>
#include <stdlib.h>
#include "src/adjacencyMatrix/adjacencyMatrix.h"
#include "src/GraphUtils/graph_utils.h"

int main () {
  Graph g;
  initGraph(&g, 5);
  insertDirectedLine(&g, 0, 1, 1.0);
  insertDirectedLine(&g, 1, 2, 2.0);
  insertDirectedLine(&g, 2, 4, 4.0);
  insertDirectedLine(&g, 4, 3, 5.0);
  insertDirectedLine(&g, 3, 1, 1.0);
  insertDirectedLine(&g, 2, 3, 2.0);
  insertDirectedLine(&g, 3, 2, 3.0);

  vertex central = 0;
  GraphCentrality(&g, &central);

  printf("Central: %d\n", central);
  return 0;
}
