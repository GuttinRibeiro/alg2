#ifndef BTREE_H
#define BTREE_H

#define BTREE_ORDER 5

#include "datahandle.hh"

template <class T>
class BTree {
private:
    struct node {
        T value;
        int dataLocation;
    };

    struct page {
        int keysNumber;
        node keys[BTREE_ORDER - 1];
        int childs[BTREE_ORDER];
        bool isLeaf;
    };

    DataHandle &_fileTemplate;
public:
    BTree(DataHandle &fileTemplate);

    int insert(node newNode);
    int find(T value);
    int remove(T value);
    void print();
};

#endif // BTREE_H
