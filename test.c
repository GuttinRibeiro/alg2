#include <stdio.h>
#include "src/List/list.h"

int main () {
  List l1;
  listInit(&l1);
  listPushFront(&l1, 1);
  listPushFront(&l1, 2);
  listPushFront(&l1, 3);
  listPushFront(&l1, 4);

  printList(&l1);
  listRemoveByElem(&l1, 2);

  printList(&l1);

  listRemoveByIndex(&l1, 2);

  printList(&l1);

  listClean(&l1);
  return 0;
}
