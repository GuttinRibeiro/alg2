#include <iostream>
#include "src/btree.hh"
#include "src/registerparser.hh"

int main(int argc, char *argv[]) {
    BTree btree("index.dat");

//    std::cout << "Search: " << btree.search(123) << "\n";
//    btree.insert(123, 4567);


//    BTree::node_t node;
//    node.keyNumber = 1;
//    node.keys[0].value = 123;
//    node.keys[0].offset = 159;
//    node.links[0] = 456;
//    node.links[1] = 789;

    btree.openIndexFile();
//    btree.setRootRRNOnFile(0);
//    btree.writeNode(node);


//    std::cout << "Root RRN: " << btree.getRootRRNFromFile() << "\n";
//    std::cout << "Current pointer: " << btree._indexStream.tellp() << "\n";

    btree._indexStream.seekg(8);
//    BTree::node_t readNode = btree.getNodeFromFile();

//    std::cout << "keyNumber: " << readNode.keyNumber << "\n";
//    std::cout << "key 0 value: " << readNode.keys[0].value << "\n";
//    std::cout << "key 0 offset: " << readNode.keys[0].offset << "\n";
//    std::cout << "link 0: " << readNode.links[0] << "\n";
//    std::cout << "link 1: " << readNode.links[1] << "\n";

    btree._indexStream.close();

    return 0;
}
