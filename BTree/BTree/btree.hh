#ifndef BTREE_H
#define BTREE_H

#include "filetemplate.hh"

class BTree {
private:
    unsigned int _order;
    FileTemplate &_fileTemplate;
public:
    BTree(unsigned int order, FileTemplate &fileTemplate);
};

#endif // BTREE_H
