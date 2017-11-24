#include "btree.hh"

BTree::BTree(const char *indexFile) {
    _indexFile = indexFile;
}

BTree::~BTree() {
    if(_indexStream.is_open()) {
        _indexStream.close();
    }
}

bool BTree::openIndexFile() {
    if(_indexStream.is_open() == false) {
        _indexStream.open(_indexFile, ios::out // open as write
                                    | ios::in  // and read file
                                    | ios::binary); // set as a binary file
    }

    return _indexStream.is_open();
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

const char *BTree::keyParser(key_t &key) {
    // key is like [value][offset]
    // copy value
    memcpy( _keyBuffer, &(key.value), sizeof(key.value) );

    // copy offset
    memcpy( &(_keyBuffer[ sizeof(key.value) ]), &(key.offset), sizeof(key.offset) );

    return _keyBuffer;
}

BTree::key_t &BTree::keyParser(const char *key) {
    // key is like [value][offset]
    // copy value
    memcpy( &(_key.value), key, sizeof(_key.value) );

    // copy offset
    memcpy( &(_key.offset), &(key[sizeof(_key.value)]), sizeof(_key.offset) );

    return _key;
}

size_t BTree::keyParser(char *dest, key_t &key) {
    // key is like [value][offset]
    // copy value
    memcpy( dest, &(key.value), sizeof(key.value) );

    // copy offset
    memcpy( &(dest[ sizeof(key.value) ]), &(key.offset), sizeof(key.offset) );

    return (sizeof(key));
}

size_t BTree::keyParser(key_t &dest, const char *src) {
    dest = keyParser(src);

    return (sizeof(dest));
}

/* node */
offset_t BTree::nodeOffset(rrn_t rrn) {
    // convert rrn to offset
    // offset = rrn * sizeof(node) + sizeof(rootNode)
    return rrn*NODE_SIZE + sizeof(rrn);
}

rrn_t BTree::getRootRRNFromFile() {
    if( _indexStream.is_open() ) {
        _indexStream.seekg(0);

        rrn_t rootRRN = INVALID_RRN;
        _indexStream.read((char*)&rootRRN, sizeof(rootRRN));

        return rootRRN;
    } else {
        cout << "[Warning] BTree::getRootRRNFromFile() receive a call with a close file.\n";
        return INVALID_RRN;
    }
}

void BTree::setRootRRNOnFile(rrn_t rootRRN) {
    if( _indexStream.is_open() ) {
        _indexStream.seekg(0);

        _indexStream.write((char*)&rootRRN, sizeof(rootRRN));
    } else {
        cout << "[Warning] BTree::setRootRRNOnFile() receive a call with a close file.\n";
    }
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

void BTree::writeNode(node_t &node) {
    if( _indexStream.is_open() ) {
        _indexStream.seekp(0, _indexStream.end);

        _indexStream.write(nodeParser(node), sizeof(node));
    } else {
        cout << "[Warning] BTree::writeNode() receive a call with a close file.\n";
    }
}

BTree::node_t &BTree::getNodeFromFile() {
    return nodeParser(readNode());
}

const char *BTree::nodeParser(node_t &node) {
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
    linkParser( &(_nodeBuffer[bufferPointer]), node.links[node.keyNumber] );

    return _nodeBuffer;
}

BTree::node_t &BTree::nodeParser(const char *node) {
    // the node is kept like [length][link][key][link][key]...[link]
    bufferptr_t bufferPointer = 0;

    // read length
    memcpy( &(_node.keyNumber), node, sizeof(_node.keyNumber) );
    bufferPointer += sizeof(_node.keyNumber);

    if(_node.keyNumber >= ORDER) {
        std::cout << "[Warning] BTree::nodeParser(const char *node) receive a corrupted or invalid node.\n";
        _node.keyNumber = BTREE_INVALID_KEY_NUMBER;
        return _node;
    }

    // there are BTREE_KEY_NUMBER segments like [link][key]
    int i;
    for(i = 0; i < BTREE_KEY_NUMBER; i++) {
        // copy link i
        bufferPointer += linkParser(_node.links[i], &(node[bufferPointer]));

        // copy key i
        bufferPointer += keyParser(_node.keys[i], &(node[bufferPointer]));
    }

    // copy last link
    bufferPointer += linkParser(_node.links[_node.keyNumber], &(node[bufferPointer]));

    return _node;
}

int BTree::insert(int id, offset_t offset) {
    // search where to put the key
    _history.deleteAll(); // cleans up the _history
    offset_t dataOffset = search(id, true);

    if(dataOffset == INVALID_OFFSET) { // everything look fine
        // create the new key
        key_t key;
        key.value = id;
        key.offset = offset;
        // and the right link that will go with it
        rrn_t *rightLink = NULL;
        // and the root rrn in case that we have to update it
        rrn_t *rootRRN = NULL;

        bool updateNode = true;

        nodeInfo_t *nodeInfo = NULL;
        while(updateNode && _history.isEmpty() == false) { // recursion??!! fuck it!
            nodeInfo = _history.takeFirst();

            // search where to put it
            int i = 0;
            while( i < nodeInfo->node.keyNumber && key.value > nodeInfo->node.keys[i].value ) {
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
                nodeInfo->node.keys[i] = key;
                nodeInfo->node.links[i+1] = (rightLink == NULL)? INVALID_RRN : *rightLink;

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

                _updateNodes.push_front(newNode); // check the newNode to be added to the index file

                // copy half of the current node to the new node
                int i;
                for(i = (ORDER/2)+1; i < BTREE_KEY_NUMBER; i++) {
                    // copy the link and key
                    newNode->node.links[i - ((ORDER/2)+1)] = nodeInfo->node.links[i];
                    newNode->node.keys[i - ((ORDER/2)+1)] = nodeInfo->node.keys[i];

                    // cleans the link
                    nodeInfo->node.links[i] = INVALID_RRN;

                    // update number of keys
                    (nodeInfo->node.keyNumber)--;
                    (newNode->node.keyNumber)++;
                }

                //copy the last link
                newNode->node.links[i - ((ORDER/2)+1)] = nodeInfo->node.links[i];

                key = nodeInfo->node.keys[(ORDER/2)];
                rightLink = &(newNode->rrn); // this will altocorrect when we update the rrn from the new node
                                             // a small problem comes when we have to think about the call of delete
                updateNode = true;
            }

            _updateNodes.push_front(nodeInfo);
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

            _updateNodes.push_front(newNode); // check the newNode to be added to the index file

            rootRRN = &(newNode->rrn);

            newNode->node.keyNumber = 1;
            newNode->node.keys[0] = key;

            if(nodeInfo == NULL) {
                newNode->node.links[0] = INVALID_RRN;
            } else {
                newNode->node.links[0] = nodeInfo->rrn;
            }

            if(rightLink == NULL) {
                newNode->node.links[1] = INVALID_RRN;
            } else {
                newNode->node.links[1] = *rightLink;
            }

            newNode->rrn = INVALID_OFFSET;
        }

        _history.deleteAll();

        // update nodes, from here, _history will be our garbage collector

        if(openIndexFile() == false) { // something went wront when oppening the file
            cout << "[Error] BTree::insert coudn't open the index file " << "\"" << _indexFile << "\"" << ".\n";
            return -1;
        }

        while(_updateNodes.isEmpty() == false) {
            nodeInfo_t *nodeInfo = _updateNodes.first();

            if(nodeInfo->rrn == INVALID_RRN) {
                // set the pointer on the end of file
                _indexStream.seekp(0, _indexStream.end);
                // update the rrn, this will be used in case of split
                if(_indexStream.tellp() < sizeof(*rootRRN)) { // file is empty;

                    _indexStream.seekp(0);

                    if(rootRRN != NULL) {
                        _indexStream.write((char *)rootRRN, sizeof(*rootRRN)); // rewrite root
                    } else {
                        rrn_t rrn = 0;
                        _indexStream << rrn;
                    }

                    rootRRN = NULL;
                }

                nodeInfo->rrn = _indexStream.tellp()/NODE_SIZE;
            } else {
                // set the pointer on the index
                _indexStream.seekp(nodeInfo->rrn*NODE_SIZE);

            }

            // write the node
            writeNode(nodeInfo->node);

            // set as unused
            _history.push_front(nodeInfo);
            _updateNodes.pop_front();
        }

        if(rootRRN != NULL) {
            _indexStream.seekp(0);
            _indexStream.write((char *)rootRRN, sizeof(*rootRRN)); // rewrite root
        }

    } else { // it seen that our key aready exists
        std::cout << "[Warning] BTree::insert receive an existing key.\n";
    }

    // delete everything
    _history.deleteAll();

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

        rrn_t rootRRN = INVALID_RRN;
        rootRRN = getRootRRNFromFile();

        bool found = false;

        while((found == false || rootRRN != INVALID_RRN) && _indexStream.eof() == false) {
            _indexStream.seekg(nodeOffset(rootRRN)); // seek the node

            node_t &node = getNodeFromFile(); // read the node

            if(makeHistory) {
                nodeInfo_t *nodeInfo = new nodeInfo_t; // this solve the problem if the node is too big
                nodeInfo->node = node; // welll, this has to be a copy, because its a reference to _node
                nodeInfo->rrn = rootRRN;

                _history.push_front(nodeInfo); // add to _hystory
            }

            int i;
            for(i = 0; i < node.keyNumber; i++) {
                if(node.keys[i].value == key) {
                    found = true;
                    result = node.keys[i].offset;
                } else {
                    if(key < node.keys[i].value) {
                        rootRRN = node.links[i];
                    }
                }
            }

            // test if key is greater than the last key of node
            if(key > node.keys[node.keyNumber].value) {
                rootRRN = node.links[node.keyNumber];
            }
        }
    } else {
        cout << "[Error] BTree::insert coudn't open the index file " << "\"" << _indexFile << "\"" << ".\n";
        return INVALID_OFFSET;
    }

    _indexStream.close();

    return result;
}

offset_t BTree::search(int key) {
    return search(key, false);
}
