#ifndef LIST_H
#define LIST_H

typedef (void *) elemType

typedef struct _listNode {
  elemType value;
  struct _listNode *next;
} listNode;

typedef struct _simpleList {
  listNode *first;
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

#endif