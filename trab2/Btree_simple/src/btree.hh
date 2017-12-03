#ifndef BTREE_HH
#define BTREE_HH

#include "sharedheader.hh"
#include "list.hh"
#include "loghandle.hh"
#include "datahandle.hh"

#define BTREE_MIN_KEY_NUM ((ORDER-1)/2)
#define BTREE_KEY_NUMBER (ORDER-1)
#define BTREE_INVALID_KEY_NUMBER (-1)
#define BTREE_SPLIT_INDEX (ORDER/2)

using namespace std;

class BTree {
friend class DataHandle;

private:
    struct Header {
        static constexpr char headerMsg[] = {'B', 'T'};
        rrn_t RRNCounter;
        rrn_t rootRRN;

        enum state {
            UPDATED = 1,
            REVIEW = 0
        };

        char updated;
    };

    struct Key {
        int value;
        offset_t offset;
    };

    struct Node {
        int keyNumber;
        Key keys[BTREE_KEY_NUMBER];
        rrn_t links[ORDER];
    };

    struct nodeInfo_t {
        Node node;
        rrn_t rrn;
        int idx;
    };

    enum linkSide {
        LEFT_SIDE = 0,
        RIGHT_SIDE = 1
    };

    /* file */
    const char *_indexFile;
    fstream _indexStream;
    int _isFileOpened;

    Header _header;
    bool _isHeaderUpdated;

    void setRootRRN(rrn_t rrn);
    rrn_t rootRRN();

    void setUpdated(Header::state updated);
    Header::state isUpdated();

    rrn_t bindAvailableRRN();
    rrn_t RRNCounter();

    void writeHeader();
    void readHeader();

    bool openIndexFile();
    void closeIndexFile();

    /* key */
    char *_keyBuffer;
    Key *_key;
    void flushKeyBuffer();

    /* node */
    char *_nodeBuffer;
    Node *_node;
    void flushNodeBuffer();

    /* insert */
    offset_t search(int key, bool makeHistory);
    List<nodeInfo_t *> _history;
    List<nodeInfo_t *> _updateNodes;

    int applyUpdates();

    List<rrn_t> _printQueue;

    LogHandle *_log;
    LogHandle &log() { return *_log; }

    void printNode(Node &node);

    int insertWithoutSearch(int id, offset_t offset);
public:
    BTree(const char *indexFile, LogHandle *log);
    ~BTree();

    const char *indexPath() { return _indexFile; }

    /* link function */
    size_t linkParser(char *dest, rrn_t &link);
    size_t linkParser(rrn_t &dest, const char *src);

    /* key functions */
    const char *keyParser() { return _keyBuffer; }
    const char *keyParser(Key &key);
    Key &keyParser(const char *key);
    size_t keyParser(char *dest, Key &key);
    size_t keyParser(Key &dest, const char *src);
    void removeKeyFromNode(Node &node, int keyIdx, linkSide linkSideIdx);
    int appendKeyToNode(Node &node, Key &key, rrn_t rightLink);
    int prependKeyToNode(Node &node, Key &key, rrn_t leftLink);

    /* node functions */
    offset_t toNodeOffset(rrn_t rrn);
    rrn_t toNodeRRN(offset_t offset);

    const char *readNode();
    const char *readNode(rrn_t rrn);
    void writeNode(Node &node);

    // load node into buffer
    Node &loadNode();
    Node &loadNodeAt(rrn_t rrn);
    // put node into a destination
    void getNode(Node &dest);
    void getNodeAt(Node &dest, rrn_t rrn);

    const char *nodeParser() { return _nodeBuffer; }
    const char *nodeParser(Node &node);
    Node &nodeParser(const char *node);
    void decodeNodeTo(Node &dest, const char *src);

    int insert(int id, offset_t offset);
    offset_t search(int key);
    offset_t remove(int id);

    void print();
};

#endif // BTREE_HH
