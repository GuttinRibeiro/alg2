#include <iostream>
#include "src/btree.hh"
#include "src/registerparser.hh"

int main(int argc, char *argv[]) {
    BTree btree("index.dat");

    std::cout << "Search: " << btree.search(-1) << "\n";
//    btree.insert(-1, 4);


//    BTree::node_t node;
//    node.keyNumber = 1;
//    node.keys[0].value = 123;
//    node.keys[0].offset = 159;
//    node.links[0] = 456;
//    node.links[1] = 789;

    btree.openIndexFile();
//    btree.setRootRRNOnFile(0);
//    btree.writeNode(node);


    btree.readHeader();
//    std::cout << "Current pointer: " << btree._indexStream.tellp() << "\n";
    std::cout << btree._header.rootRRN << "\n";

//    btree._indexStream.seekg(18);
    BTree::Node readNode;
    readNode = btree.loadNode();

    std::cout << "keyNumber: " << readNode.keyNumber << "\n";

    for(int i = 0; i < BTREE_KEY_NUMBER; i++) {
        std::cout << "link " << i << ": " << readNode.links[i] << "\n";
        std::cout << "key " << i << " value: " << readNode.keys[i].value << "\n";
        std::cout << "key " << i << " offset: " << readNode.keys[i].offset << "\n";
    }
    std::cout << "link " << 4 << ": " << readNode.links[4] << "\n";
    readNode = btree.loadNode();

    std::cout << "keyNumber: " << readNode.keyNumber << "\n";

    for(int i = 0; i < BTREE_KEY_NUMBER; i++) {
        std::cout << "link " << i << ": " << readNode.links[i] << "\n";
        std::cout << "key " << i << " value: " << readNode.keys[i].value << "\n";
        std::cout << "key " << i << " offset: " << readNode.keys[i].offset << "\n";
    }
    std::cout << "link " << 4 << ": " << readNode.links[4] << "\n";
    readNode = btree.loadNode();

    std::cout << "keyNumber: " << readNode.keyNumber << "\n";

    for(int i = 0; i < BTREE_KEY_NUMBER; i++) {
        std::cout << "link " << i << ": " << readNode.links[i] << "\n";
        std::cout << "key " << i << " value: " << readNode.keys[i].value << "\n";
        std::cout << "key " << i << " offset: " << readNode.keys[i].offset << "\n";
    }
    std::cout << "link " << 4 << ": " << readNode.links[4] << "\n";

    btree._indexStream.close();

    return 0;
}
