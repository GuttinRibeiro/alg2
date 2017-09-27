#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef int elemType;
typedef int listSize;

typedef struct _listNode {
  elemType value;
  struct _listNode *next;
} listNode;

typedef struct _simpleList {
  listNode *first;
  listSize size;
} List;

/** listInit: initiate the list (*l)
 ** Complexity: O(1)
 */
int listInit(List *l);

/** listPushFront: add element elem in the front of the list
 ** Complexity: O(1)
 */
int listPushFront(List *l, elemType elem);

/** listRemoveByElem: remove first elem from the list
 ** Complexity: O(size)
 */
int listRemoveByElem(List *l, elemType elem);

/** listRemoveByIndex: remove element of index index
 ** Complexity: O(size)
 */
int listRemoveByIndex(List *l, unsigned int index);

/** listClean: remove all elements
 ** Complexity: O(size)
 */
int listClean(List *l);

/* Iterator */
typedef listNode* listIterator;

/** itrBegin initiate list iterator
 ** Complexity: O(1)
 */
listIterator itrBegin(List *l);

/** itrEnd returns the final
 ** Complexity: O(1)
 */
listIterator itrEnd();

/** itrNext walks on the list
 ** Complexity: O(1)
 */
void itrNext(listIterator *it);

/** itrJump walks jumpSize elements on the list
 ** Complexity: O(jumpSize)
 */
unsigned int itrJump(listIterator *it, unsigned int jumpSize);

/** itrValue returns the value of the current element
 ** Complexity: O(1)
 */
elemType itrValue(listIterator it);

#endif
