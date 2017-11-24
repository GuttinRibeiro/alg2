#ifndef BTREE_HH
#define BTREE_HH

#include "commonheader.h"
#include "list.hh"

#define BTREE_KEY_NUMBER (ORDER-1)
#define BTREE_INVALID_KEY_NUMBER (-1)

using namespace std;

class BTree {
public:
    struct key_t {
        int value;
        offset_t offset;
    };

    struct node_t {
        int keyNumber;
        key_t keys[BTREE_KEY_NUMBER];
        rrn_t links[ORDER];
    };

    struct nodeInfo_t {
        node_t node;
        rrn_t rrn;
    };

    const char *_indexFile;
    fstream _indexStream;

    bool openIndexFile();

    /* key */
    char _keyBuffer[KEY_SIZE];
    key_t _key;
    void flushKeyBuffer();

    /* node */
    char _nodeBuffer[NODE_SIZE];
    node_t _node;
    void flushNodeBuffer();

    /* insert */
    offset_t search(int key, bool makeHistory);
    List<nodeInfo_t *> _history;
    List<nodeInfo_t *> _updateNodes;
public:
    BTree(const char *indexFile);
    ~BTree();

    /* link function */
    size_t linkParser(char *dest, rrn_t &link);
    size_t linkParser(rrn_t &dest, const char *src);

    /* key functions */
    const char *keyParser() { return _keyBuffer; }
    const char *keyParser(key_t &key);
    key_t &keyParser(const char *key);
    size_t keyParser(char *dest, key_t &key);
    size_t keyParser(key_t &dest, const char *src);

    /* node functions */
    offset_t nodeOffset(rrn_t rrn);
    rrn_t getRootRRNFromFile();
    void setRootRRNOnFile(rrn_t rootRRN);
    const char *readNode();
    void writeNode(node_t &node);
    node_t &getNodeFromFile();
    const char *nodeParser() { return _nodeBuffer; }
    const char *nodeParser(node_t &node);
    node_t &nodeParser(const char *node);

    int insert(int id, offset_t offset);
    offset_t search(int key);
    int remove(int id);
};

#endif // BTREE_HH
