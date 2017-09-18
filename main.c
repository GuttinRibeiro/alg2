#include <stdio.h>
#include <stdlib.h>
#include "adjacencyList.h"
//#include "adjacencyMatrix.h"

int main () {
  Graph g;
  initGraph(&g, 3);
//  printGraph(&g);

  block u1, u2, u3;
  initBlock(&u1, 1.3, 0);
  //printBlock(&u1);
  initBlock(&u2, 0.7, 1);
  //printBlock(&u2);
  initBlock(&u3, 0.4, 2);
  //printBlock(&u3);

  insertLine(&g, &u1, &u2);
  //printGraph(&g);
  insertLine(&g, &u1, &u3);
  //printGraph(&g);
  /*printBlock(&u1);
  printBlock(&u2);
  printBlock(&u3);*/
  insertLine(&g, &u2, &u3);
  //printGraph(&g);

  return 0;
}
