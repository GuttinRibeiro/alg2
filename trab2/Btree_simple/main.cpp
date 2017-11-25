#include <iostream>
#include "src/btree.hh"
#include "src/registerparser.hh"

int main(int argc, char *argv[]) {
    BTree btree("index.dat");

//    std::cout << "Search: " << btree.search(-1) << "\n";
    btree.insert(1, 1);
    btree.insert(2, 2);
    btree.insert(3, 3);
    btree.insert(4, 4);
    btree.insert(5, 5);
    btree.insert(6, 6);
    btree.insert(7, 7);
    btree.insert(8, 8);
    btree.insert(9, 9);
    btree.insert(10, 10);
    btree.insert(11, 11);
    btree.insert(12, 12);
    btree.insert(13, 13);
    btree.insert(14, 14);
    btree.insert(15, 15);
    btree.insert(16, 16);
    btree.insert(17, 17);
    btree.insert(18, 18);
    btree.insert(19, 19);
    btree.insert(20, 20);
    btree.insert(21, 21);
    btree.insert(22, 22);
    btree.insert(23, 23);
    btree.insert(24, 24);
    btree.insert(25, 25);
    btree.insert(26, 26);
    btree.insert(27, 27);
    btree.insert(28, 28);
    btree.insert(29, 29);
    btree.insert(30, 30);


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

    for(int j = 0; j < 15; j++) {
        readNode = btree.loadNode();

        std::cout << "keyNumber: " << readNode.keyNumber << "\n";

        for(int i = 0; i < BTREE_KEY_NUMBER; i++) {
            std::cout << "link " << i << ": " << readNode.links[i] << "\n";
            std::cout << "key " << i << " value: " << readNode.keys[i].value << "\n";
            std::cout << "key " << i << " offset: " << readNode.keys[i].offset << "\n";
        }
        std::cout << "link " << 4 << ": " << readNode.links[4] << "\n\n\n\n\n";
    }

    btree._indexStream.close();

    return 0;
}
