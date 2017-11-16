#include "btree.hh"

BTree::BTree(unsigned int order, FileTemplate &fileTemplate) : _fileTemplate(fileTemplate) {
    _order = order;
}

