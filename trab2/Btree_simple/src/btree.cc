#include "btree.hh"

constexpr char BTree::Header::headerMsg[];

BTree::BTree(const char *indexFile) {
    _indexFile = indexFile;
    _isFileOpened = 0;
    _isHeaderUpdated = false;

    _header.RRNCounter = 0;
    _header.rootRRN = -1;

    // test file
    if(openIndexFile()) {
        // test if the file is empty
        if(_indexStream.peek() == ifstream::traits_type::eof()) {
            cerr << "[Warning] BTree receive an empty file. Overwriting.\n";

            // test if we can write on the file
            if(!(_indexStream.write(Header::headerMsg, 2) )) {
                cerr << "[Error] BTree couldn't write on file.\n";
                abort();
            }

            // write default header
            writeHeader();
        } else {
            // file isn't empty

            // test if we can read the file
            char headerMsg[2];
            if( !(_indexStream.read(headerMsg, 2)) ) {
                cerr << "[Error] BTree couldn't read the file.\n";
                abort();
            }

            // test if we can write on the file
            _indexStream.seekp(0);
            if(!(_indexStream.write(headerMsg, 2) )) {
                cerr << "[Error] BTree couldn't write on file.\n";
                abort();
            }

            // test if the file was created by us
            if(strncmp(headerMsg, Header::headerMsg, 2) == 0) {
                readHeader();
            } else {
                cerr << "[Warning] BTree receive an corrupted file.\n";
            }
        }

    } else { // file dosn't exist
        cerr << "[Warning] BTree coudn't open index file, creating a new one at \"" << indexFile << "\".\n";
        _indexStream.open(_indexFile, fstream::out // open as write
                                    | fstream::binary); // set as a binary file

        // test if we can write on the file
        if(!(_indexStream.write(Header::headerMsg, 2) )) {
            cerr << "[Error] BTree couldn't write on file.\n";
            abort();
        }

        // write default header
        writeHeader();
    }

    closeIndexFile();

    // create buffer
    _keyBuffer = new char[KEY_SIZE];
    _key = new Key;
    _nodeBuffer = new char[NODE_SIZE];
    _node = new Node;

    // TODO: handle null
}

BTree::~BTree() {
    if(_indexStream.is_open()) {
        _indexStream.close();
    }

    // delete buffers
    delete _keyBuffer;
    delete _key;
    delete _nodeBuffer;
    delete _node;
}

void BTree::setRootRRN(rrn_t rrn) {
    _header.rootRRN = rrn;
    _isHeaderUpdated = false;
}

rrn_t BTree::bindAvailableRRN() {
    _isHeaderUpdated = false;

    return (_header.RRNCounter)++;
}

rrn_t BTree::rootRRN() {
    return _header.rootRRN;
}

rrn_t BTree::RRNCounter() {
    return _header.RRNCounter;
}

void BTree::writeHeader() {
    openIndexFile();

    _indexStream.seekp(2);
    _indexStream.write((char *)&(_header.RRNCounter), sizeof(_header.RRNCounter));
    _indexStream.write((char *)&(_header.rootRRN), sizeof(_header.rootRRN));

    closeIndexFile();

    _isHeaderUpdated = true;
}

void BTree::readHeader() {
    openIndexFile();

    _indexStream.seekg(2);
    _indexStream.read((char *)&(_header.RRNCounter), sizeof(_header.RRNCounter));
    _indexStream.read((char *)&(_header.rootRRN), sizeof(_header.rootRRN));

    closeIndexFile();

    _isHeaderUpdated = true;
}

bool BTree::openIndexFile() {
    // handles the oppening if the file is aready open
    if(_indexStream.is_open() == false) {
        _indexStream.open(_indexFile, fstream::out // open as write
                                    | fstream::in  // and read file
                                    | fstream::binary); // set as a binary file

        _isFileOpened = 1;
    } else {
        _isFileOpened++;
    }

    return _indexStream.is_open();
}

void BTree::closeIndexFile() {
    // close the file only if the function is the one
    // that open it
    if(_isFileOpened == 1) {
        _indexStream.close();
    }

    _isFileOpened--;
}

/* link */
size_t BTree::linkParser(char *dest, rrn_t &link) {
    memcpy(dest, &link, sizeof(link));

    return sizeof(link);
}

size_t BTree::linkParser(rrn_t &dest, const char *src) {
    memcpy( &dest, src, sizeof(dest) );

    return sizeof(dest);
}

/* key */
void BTree::flushKeyBuffer() {
    memset(_keyBuffer, 0, KEY_SIZE); // clean _keyBuffer
}

const char *BTree::keyParser(Key &key) {
    // key is like [value][offset]
    // copy value
    memcpy( _keyBuffer, &(key.value), sizeof(key.value) );

    // copy offset
    memcpy( &(_keyBuffer[ sizeof(key.value) ]), &(key.offset), sizeof(key.offset) );

    return _keyBuffer;
}

BTree::Key &BTree::keyParser(const char *key) {
    // key is like [value][offset]
    // copy value
    memcpy( &(_key->value), key, sizeof(_key->value) );

    // copy offset
    memcpy( &(_key->offset), &(key[sizeof(_key->value)]), sizeof(_key->offset) );

    return *_key;
}

size_t BTree::keyParser(char *dest, Key &key) {
    // key is like [value][offset]
    // copy value
    memcpy( dest, &(key.value), sizeof(key.value) );

    // copy offset
    memcpy( &(dest[ sizeof(key.value) ]), &(key.offset), sizeof(key.offset) );

    return (sizeof(key));
}

size_t BTree::keyParser(Key &dest, const char *src) {
    dest = keyParser(src);

    return (sizeof(dest));
}

/* node */
offset_t BTree::toNodeOffset(rrn_t rrn) {
    // convert rrn to offset
    // offset = rrn * sizeof(node) + sizeof(header) + 2
    return rrn*NODE_SIZE + sizeof(Header) + 2;
}

rrn_t BTree::toNodeRRN(offset_t offset) {
    return (offset - sizeof(Header) - 2)/NODE_SIZE;
}

void BTree::flushNodeBuffer() {
    memset(_nodeBuffer, 0, NODE_SIZE);  // clean _nodeBuffer
}

const char *BTree::readNode() {
    if( _indexStream.is_open() ) {
        _indexStream.read(_nodeBuffer, NODE_SIZE);
        return _nodeBuffer;
    } else {
        cout << "[Warning] BTree::readNode() receive a call with a close file.\n";
        flushNodeBuffer();
        return _nodeBuffer;
    }
}

void BTree::writeNode(Node &node) {
    if( _indexStream.is_open() ) {
        _indexStream.write(nodeParser(node), sizeof(node));
    } else {
        cout << "[Warning] BTree::writeNode() receive a call with a close file.\n";
    }
}

BTree::Node &BTree::loadNode() {
    return nodeParser(readNode());
}

void BTree::getNode(Node &dest) {
    decodeNodeTo(dest, readNode());
}

const char *BTree::nodeParser(Node &node) {
    // the node is kept like [length][link][key][link][key]...[link]

    if(node.keyNumber >= ORDER) {
        std::cout << "[Warning] BTree::nodeParser(node_t &node) receive a corrupted or invalid node.\n";
        flushNodeBuffer();
        return _nodeBuffer;
    }

    bufferptr_t bufferPointer = 0;

    // node length
    memcpy( _nodeBuffer, &(node.keyNumber), sizeof(node.keyNumber) );
    bufferPointer += sizeof(node.keyNumber);

    // there are BTREE_KEY_NUMBER segments like [link][key]
    int i;
    for(i = 0; i < BTREE_KEY_NUMBER; i++) {
        // copy link i
        bufferPointer += linkParser( &(_nodeBuffer[bufferPointer]), node.links[i] );

        // copy key i
        bufferPointer += keyParser( &(_nodeBuffer[bufferPointer]), node.keys[i]);
    }

    // copy last link
    linkParser( &(_nodeBuffer[bufferPointer]), node.links[BTREE_KEY_NUMBER] );

    return _nodeBuffer;
}

BTree::Node &BTree::nodeParser(const char *node) {
    decodeNodeTo(*_node, node);

    return *_node;
}

void BTree::decodeNodeTo(Node &dest, const char *src) {
    // the node is kept like [length][link][key][link][key]...[link]
    bufferptr_t bufferPointer = 0;

    // read length
    memcpy( &(dest.keyNumber), src, sizeof(dest.keyNumber) );
    bufferPointer += sizeof(_node->keyNumber);

    if(dest.keyNumber >= ORDER) {
        std::cout << "[Warning] BTree::decodeNodeTo receive a corrupted or invalid source.\n";
        dest.keyNumber = BTREE_INVALID_KEY_NUMBER;
    }

    // there are BTREE_KEY_NUMBER segments like [link][key]
    // (even if the node isn't full)
    int i;
    for(i = 0; i < BTREE_KEY_NUMBER; i++) {
        // copy link i
        bufferPointer += linkParser(dest.links[i], &(src[bufferPointer]));

        // copy key i
        bufferPointer += keyParser(dest.keys[i], &(src[bufferPointer]));
    }

    // copy last link
    linkParser(dest.links[BTREE_KEY_NUMBER], &(src[bufferPointer]));
}

int BTree::insert(int id, offset_t offset) {
    // search where to put the key
    _history.deleteAll(); // cleans up the _history
    offset_t dataOffset = search(id, true);

    if(dataOffset == INVALID_OFFSET) { // everything look fine
        // create the new key
        Key *key = new Key;
        key->value = id;
        key->offset = offset;
        // and the right link that will go with it
        rrn_t rightLink = INVALID_RRN;

        bool updateNode = true;

        nodeInfo_t *nodeInfo = NULL;
        while(updateNode && _history.isEmpty() == false) { // recursion??!! fuck it!
            nodeInfo = _history.takeFirst();

            // search where to put it
            int i = 0;
            while( i < nodeInfo->node.keyNumber && key->value > nodeInfo->node.keys[i].value ) {
                i++;
            }

            // put it
            if(nodeInfo->node.keyNumber < BTREE_KEY_NUMBER) {
                // we have to maintain the order, therefore...
                int j = nodeInfo->node.keyNumber;
                while(j > i) { // move everyone to the next index
                    nodeInfo->node.keys[j] = nodeInfo->node.keys[j-1];
                    nodeInfo->node.links[j+1] = nodeInfo->node.links[j];
                    j--;
                }
                nodeInfo->node.keys[i] = *key;

                nodeInfo->node.links[i+1] = rightLink;

                (nodeInfo->node.keyNumber)++;

                updateNode = false; // there no need to keep updating the nodes...
            } else { // split

                // create a new node
                nodeInfo_t *newNode = new nodeInfo_t;
                if(newNode == NULL) { // we ran out of memory? what to do?
                    std::cout << "[Error] BTree::insert fail to allocate memory.\n";
                    // clean up memory
                    _history.deleteAll();
                    _updateNodes.deleteAll();
                    return -2;
                }

                newNode->node.keyNumber = 0;

                _updateNodes.push_back(newNode); // check the newNode to be added to the index file

                // copy half of the current node to the new node

                int i;
                for(i = BTREE_SPLIT_INDEX; i < BTREE_KEY_NUMBER; i++) {
                    // copy the link and key
                    newNode->node.links[i - BTREE_SPLIT_INDEX] = nodeInfo->node.links[i];
                    newNode->node.keys[i - BTREE_SPLIT_INDEX] = nodeInfo->node.keys[i];

                    // clean the link
                    nodeInfo->node.links[i] = INVALID_RRN;

                    // update number of keys
                    (nodeInfo->node.keyNumber)--;
                    (newNode->node.keyNumber)++;
                }

                // copy the last key
                newNode->node.links[i - BTREE_SPLIT_INDEX] = nodeInfo->node.links[i];
                // clean the link
                nodeInfo->node.links[i] = INVALID_RRN;

                // clean the rest of the links of newNode;
                for(i = i - BTREE_SPLIT_INDEX+1; i < ORDER; i++) {
                    newNode->node.links[i] = INVALID_RRN;
                }

                *key = nodeInfo->node.keys[BTREE_SPLIT_INDEX - 1];
                (nodeInfo->node.keyNumber)--;

                newNode->rrn = bindAvailableRRN();
                rightLink = newNode->rrn;

                updateNode = true;
            }

            _updateNodes.push_back(nodeInfo);
        }

        if(updateNode) {
            // create a new node
            nodeInfo_t *newNode = new nodeInfo_t;
            if(newNode == NULL) { // we ran out of memory? what to do?
                std::cout << "[Error] BTree::insert fail to allocate memory.\n";
                // clean up memory
                _history.deleteAll();
                _updateNodes.deleteAll();
                return -2;
            }

            _updateNodes.push_back(newNode); // check the newNode to be added to the index file

            newNode->node.keyNumber = 1;
            newNode->node.keys[0] = *key;

            if(nodeInfo == NULL) { // first node
                newNode->node.links[0] = INVALID_RRN;
            } else {
                newNode->node.links[0] = nodeInfo->rrn;
            }

            newNode->node.links[1] = rightLink;

            int i;
            for(i = 2; i < ORDER; i++) {
                newNode->node.links[i] = INVALID_RRN;
            }

            newNode->rrn = bindAvailableRRN();

            setRootRRN(newNode->rrn);
        }

        _history.deleteAll();

        // update nodes, from here, _history will be our garbage collector

        if(openIndexFile() == false) { // something went wront when oppening the file
            cout << "[Error] BTree::insert coudn't open the index file " << "\"" << _indexFile << "\"" << ".\n";
            return -1;
        }

        if(_isHeaderUpdated == false) {
            writeHeader();
        }

        while(_updateNodes.isEmpty() == false) {
            nodeInfo_t *nodeInfo = _updateNodes.takeFirst();

            if(nodeInfo->rrn == INVALID_RRN) {
                // set the pointer on the end of file
                _indexStream.seekp(0, _indexStream.end);
                // update the rrn, this will be used in case of split
                if(_indexStream.tellp() < sizeof(rrn_t)) { // file is empty;
                    setRootRRN(0);
                }

                nodeInfo->rrn = toNodeRRN(_indexStream.tellp());
            } else {
                // set the pointer on the index
                _indexStream.seekp(toNodeOffset(nodeInfo->rrn));

            }

            // write the node
            writeNode(nodeInfo->node);

            // set as unused
            _history.push_front(nodeInfo);
        }

        delete key;

    } else { // it seen that our key aready exists
        std::cout << "[Warning] BTree::insert receive an existing key.\n";
    }

    // delete everything
    _history.deleteAll();
    _updateNodes.deleteAll();

    _indexStream.close();

    return 0;
}

int BTree::remove(int id) {
    // TODO

    return 0;
}


offset_t BTree::search(int key, bool makeHistory) {
    offset_t result = INVALID_OFFSET;
    if(openIndexFile()) { // test if the file was openned sucessfuly

        rrn_t currRRN = INVALID_RRN;
        currRRN = rootRRN();

        bool found = false;

        while(found == false && currRRN != INVALID_RRN) {
            _indexStream.seekg(toNodeOffset(currRRN)); // seek the current node

            Node *node = NULL; // I wish that we could do with reference....

            if(makeHistory) {
                nodeInfo_t *nodeInfo = new nodeInfo_t; // this solve the problem if the node is too big
                getNode(nodeInfo->node); // put the node into nodeInfo->node
                node = &(nodeInfo->node); // we can do this because we won't delete this nodeInfo until the end
                nodeInfo->rrn = currRRN;

                _history.push_front(nodeInfo); // add to _history
            } else {
                node = &loadNode(); // we can do this because the function return a reference to _node
            }

            // search key inside node
            int i = 0;
            while(i < node->keyNumber && key >= node->keys[i].value) {
                if(node->keys[i].value == key) {
                    found = true;
                    result = node->keys[i].offset;
                }
                i++;
            }

            // go down into the tree
            if(node->keyNumber > 0) {
                currRRN = node->links[i];
            } else {
                currRRN = INVALID_RRN;
            }
        }
    } else {
        cout << "[Error] BTree::insert coudn't open the index file " << "\"" << _indexFile << "\"" << ".\n";
        return INVALID_OFFSET;
    }

    closeIndexFile();

    return result;
}

offset_t BTree::search(int key) {
    return search(key, false);
}
