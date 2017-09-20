#include "list.h"

/** listInit: initiate the list (*l)
 ** Complexity: O(1)
 */
int listInit(List *l) {
  l->first = NULL;
  return 0;
}

/** listPushFront: add element elem in the front of the list
 ** Complexity: O(1)
 */
int listPushFront(List *l, elemType elem) {
  listNode *newNode = (listNode *)malloc(sizeof(listNode));
  if(newNode == NULL) {
    return -1;
  }

  newNode->value = elem;
  newNode->next = l->first;
  l->first = newNode;

  return 0;
}

/** listRemoveByElem: remove first elem from the list
 ** Complexity: O(size)
 */
int listRemoveByElem(List *l, elemType elem) {
  listNode *curr = l->first;
  listNode *prev = NULL;
  while(curr != NULL && curr->value != elem) {
    prev = curr;
    curr = curr->next;
  }

  if(curr == NULL) {
    return -1;
  }

  if(prev == NULL) {
    l->first = curr->next;
  } else {
    prev->next = curr->next;
  }

  free(curr);
  return 0;
}

/** listRemoveByIndex: remove element of index index
 ** Complexity: O(size)
 */
int listRemoveByIndex(List *l, unsigned int index) {
  listNode *rm = NULL;
  if(index == 0) {
    rm = l->first;
    if(rm == NULL) {
      return -1;
    }

    l->first = rm->next;
  } else {
    listNode *curr = l->first;

    int i = 0;
    while(curr != NULL && i < index-1) {
      curr = curr->next;
      i++;
    }

    if(i != index-1) {
      return -1;
    }

    rm = curr->next;

    if(rm == NULL) {
      remove -1;
    }

    curr->next = rm->next;
  }

  free(rm);
  return 0;
}

/** listClean: remove all elements
 ** Complexity: O(size)
 */
int listClean(List *l) {
  listNode *next = NULL;
  listNode *curr = l->first;

  while(curr != NULL) {
    next = curr->next;
    free(curr);

    curr = next;
  }

  l->first = NULL;

  return 0;
}
