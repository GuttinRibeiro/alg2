#include "btree.hh"

constexpr char BTree::Header::headerMsg[];

BTree::BTree(const char *indexFile, LogHandle *log) {
    _log = log;

    _indexFile = indexFile;
    _isFileOpened = 0;
    _isHeaderUpdated = false;

    _header.RRNCounter = 0;
    _header.rootRRN = -1;
    _header.updated = Header::UPDATED;

    // test file
    if(openIndexFile()) {
        // test if the file is empty
        if(_indexStream.peek() == ifstream::traits_type::eof()) {
            cerr << "[Warning] BTree receive an empty file. Overwriting.\n";

            // for some reason, the file doesn't write with in and out
            _indexStream.close();
            _indexStream.open(indexFile, fstream::out // open as write
                                       | fstream::binary); // set as a binary file

            // test if we can write on the file
            if(!(_indexStream.write(Header::headerMsg, 2) )) {
                cerr << "[Error] BTree couldn't write on file.\n";
                abort();
            }

            // write default header
            writeHeader();
            _indexStream.close();
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

void BTree::setUpdated(Header::state updated) {
    _header.updated = updated;
    _isHeaderUpdated = false;
}

BTree::Header::state BTree::isUpdated() {
    return static_cast<BTree::Header::state>(_header.updated);
}

rrn_t BTree::RRNCounter() {
    return _header.RRNCounter;
}

void BTree::writeHeader() {
    openIndexFile();

    _indexStream.seekp(2);
    _indexStream.write((char *)&(_header.RRNCounter), sizeof(_header.RRNCounter));
    _indexStream.write((char *)&(_header.rootRRN), sizeof(_header.rootRRN));
    _indexStream.write(&(_header.updated), sizeof(_header.updated));

    closeIndexFile();

    _isHeaderUpdated = true;
}

void BTree::readHeader() {
    openIndexFile();

    _indexStream.seekg(2);
    _indexStream.read((char *)&(_header.RRNCounter), sizeof(_header.RRNCounter));
    _indexStream.read((char *)&(_header.rootRRN), sizeof(_header.rootRRN));
    _indexStream.read(&(_header.updated), sizeof(_header.updated));

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

void BTree::removeKeyFromNode(Node &node, int keyIdx, linkSide linkSideIdx) {
    if(node.keyNumber > 0 && keyIdx < node.keyNumber) {
        int i;
        for(i = keyIdx; i < node.keyNumber-1; i++) { // keep things organized
            node.keys[i] = node.keys[i+1];
            node.links[i+linkSideIdx] = node.links[i+linkSideIdx+1];
        }
//        //copy the last link
//        node.links[i] = node.links[i+1];

        // clean the last key and link
        node.keys[i].value = DEFAULT_KEY;
        node.keys[i].offset = INVALID_OFFSET;
        node.links[i+linkSideIdx] = INVALID_RRN;

        // decrement the key number
        node.keyNumber--;
    }
}

int BTree::appendKeyToNode(Node &node, Key &key, rrn_t rightLink) {
    if(node.keyNumber < BTREE_KEY_NUMBER) {
        node.keys[node.keyNumber++] = key;
        node.links[node.keyNumber] = rightLink;

        return 0;
    }
    return 1;
}

int BTree::prependKeyToNode(Node &node, Key &key, rrn_t leftLink) {
    if(node.keyNumber < BTREE_KEY_NUMBER) {
        int i = node.keyNumber;
        while(i-- > 0) {
            node.keys[i+1] = node.keys[i];
            node.links[i+2] = node.links[i+1];
        }
        node.links[1] = node.links[0];

        node.keys[0] = key;
        node.links[0] = leftLink;

        node.keyNumber++;
        return 0;
    }
    return 1;
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

const char *BTree::readNode(rrn_t rrn) {
    if( _indexStream.is_open() ) {
        _indexStream.seekg(toNodeOffset(rrn));
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

BTree::Node &BTree::loadNodeAt(rrn_t rrn) {
    return nodeParser(readNode(rrn));
}

void BTree::getNode(Node &dest) {
    decodeNodeTo(dest, readNode());
}

void BTree::getNodeAt(Node &dest, rrn_t rrn) {
    decodeNodeTo(dest, readNode(rrn));
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

int BTree::insertWithoutSearch(int id, offset_t offset) {
    // create the new key
    Key *key = new Key;
    key->value = id;
    key->offset = offset;
    // and the right link that will go with it
    rrn_t rightLink = INVALID_RRN;

    bool updateNode = true;

    nodeInfo_t *nodeInfo = NULL;
    while(updateNode && (_history.isEmpty() == false)) { // recursion??!! fuck it!
        nodeInfo = _history.takeFirst();

        if(nodeInfo->node.keyNumber < BTREE_KEY_NUMBER) {
            // place to put it
            int i = nodeInfo->idx;

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
            // How the split work... we have to pointers in the node that
            // shall suffer from being split... the first one walks increasing
            // it's value, until it find the possition that the key must go, or
            // reach the BTREE_KEY_SPLIT-1, the second do the same, but decreasing.

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
            newNode->rrn = bindAvailableRRN();
            newNode->idx = -1;

            _updateNodes.push_back(newNode);

            // search where to put the key....
            int i = nodeInfo->idx;
            if(i >= BTREE_SPLIT_INDEX) {
                i = BTREE_SPLIT_INDEX;
            }

            int j = BTREE_KEY_NUMBER - 1;
            while(j >= BTREE_SPLIT_INDEX && key->value < nodeInfo->node.keys[j].value) {
                newNode->node.keys[j - BTREE_SPLIT_INDEX] = nodeInfo->node.keys[j];
                newNode->node.links[j - BTREE_SPLIT_INDEX+1] = nodeInfo->node.links[j+1];

                nodeInfo->node.keys[j].value = DEFAULT_KEY;
                nodeInfo->node.keys[j].offset = INVALID_OFFSET;
                nodeInfo->node.links[j+1] = INVALID_RRN;

                newNode->node.keyNumber++;
                nodeInfo->node.keyNumber--;

                j--;
            }

            if(j - i >= 0) {
                if(i == BTREE_SPLIT_INDEX) {
                    if(id == 4) {
                        std::cout << "";
                    }
                    // copy the key
                    newNode->node.keys[j - BTREE_SPLIT_INDEX] = *key;
                    newNode->node.links[j - BTREE_SPLIT_INDEX+1] = rightLink;
                    j--;

                    newNode->node.keyNumber++;

                    // set the key to the next iteration
                    *key = nodeInfo->node.keys[BTREE_SPLIT_INDEX];
                    rightLink = newNode->rrn;

                    nodeInfo->node.keys[BTREE_SPLIT_INDEX].offset = INVALID_OFFSET;
                    nodeInfo->node.keys[BTREE_SPLIT_INDEX].value = DEFAULT_KEY;

                    nodeInfo->node.keyNumber--;

                    // finish the copy
                    while(j >= BTREE_SPLIT_INDEX) {
                        newNode->node.keys[j - BTREE_SPLIT_INDEX] = nodeInfo->node.keys[j+1];
                        newNode->node.links[j - BTREE_SPLIT_INDEX+1] = nodeInfo->node.links[j+2];

                        nodeInfo->node.keys[j+1].value = DEFAULT_KEY;
                        nodeInfo->node.keys[j+1].offset = INVALID_OFFSET;
                        nodeInfo->node.links[j+2] = INVALID_RRN;

                        newNode->node.keyNumber++;
                        nodeInfo->node.keyNumber--;

                        j--;
                    }
                    // copy the first link
                    newNode->node.links[0] = nodeInfo->node.links[BTREE_SPLIT_INDEX+1];
                    nodeInfo->node.links[BTREE_SPLIT_INDEX+1] = INVALID_RRN;
                } else if(j == BTREE_SPLIT_INDEX - 1) {
                    // copy the first link
                    newNode->node.links[0] = nodeInfo->node.links[BTREE_SPLIT_INDEX];
                    nodeInfo->node.links[BTREE_SPLIT_INDEX] = INVALID_RRN;

                    // spaaaceee.
                    j = BTREE_SPLIT_INDEX - 1;
                    while(j >= i) {
                        nodeInfo->node.keys[j+1] = nodeInfo->node.keys[j];
                        nodeInfo->node.links[j+2] = nodeInfo->node.links[j+1];

                        j--;
                    }

                    nodeInfo->node.keys[i] = *key;
                    nodeInfo->node.links[i+1] = rightLink;

                    // set the key to the next iteration
                    // REMEMBER... we move the keys and links to open space
                    *key = nodeInfo->node.keys[BTREE_SPLIT_INDEX];
                    rightLink = newNode->rrn;

                    nodeInfo->node.keys[BTREE_SPLIT_INDEX].offset = INVALID_OFFSET;
                    nodeInfo->node.keys[BTREE_SPLIT_INDEX].value = DEFAULT_KEY;
                }

                // clean the links
                for(j = BTREE_KEY_NUMBER - BTREE_SPLIT_INDEX; j < BTREE_KEY_NUMBER; j++) {
                    newNode->node.keys[j].value = DEFAULT_KEY;
                    newNode->node.keys[j].offset = INVALID_OFFSET;
                    newNode->node.links[j+1] = INVALID_RRN;
                }
            } else {
                // copy the first link
                newNode->node.links[0] = nodeInfo->node.links[BTREE_SPLIT_INDEX];
                nodeInfo->node.links[BTREE_SPLIT_INDEX] = INVALID_RRN;
                rightLink = newNode->rrn;
            }

            log().hold(true);
            log() << "Divisao de no - pagina " << nodeInfo->rrn << "\n"
                  << "Chave " << key->value << " promovida\n";
            log().hold(false);

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

        newNode->rrn = bindAvailableRRN();
        newNode->node.keyNumber = 1;
        newNode->node.keys[0] = *key;

        if(nodeInfo == NULL) { // first node
            newNode->node.links[0] = INVALID_RRN;
        } else {
            newNode->node.links[0] = nodeInfo->rrn;
        }

        newNode->node.links[1] = rightLink;

        int i;
        for(i = 1; i < BTREE_KEY_NUMBER; i++) {
            newNode->node.links[i+1] = INVALID_RRN;
            newNode->node.keys[i].offset = INVALID_OFFSET;
            newNode->node.keys[i].value = DEFAULT_KEY;
        }


        setRootRRN(newNode->rrn);
    }

    // delete everything
    delete key;

    int rtrn = applyUpdates();
    if( rtrn != 0) {
        return rtrn;
    }

    return 0;
}

int BTree::insert(int id, offset_t offset) {
    // search where to put the key
    _history.deleteAll(); // cleans up the _history
    offset_t dataOffset = search(id, true);

    if(dataOffset == INVALID_OFFSET) { // everything look fine
        int rtrn = insertWithoutSearch(id, offset);
        if(rtrn != 0) {
            return rtrn;
        }
    } else { // it seen that our key aready exists
        log().hold(true);
        log() << "Chave " << id << " duplicada.\n";
        log().hold(false);

        return -2;
    }

    _indexStream.close();

    log().hold(true);
    log() << "Chave " << id << " inserida com sucesso.\n";
    log().hold(false);

    return 0;
}

offset_t BTree::remove(int id) {
    log().hold(true);
    log() << "Execucao de operacao de REMOCAO de " << id << ".\n";
    log().hold(false);

    // clean all
    _history.deleteAll();
    _updateNodes.deleteAll();

    openIndexFile();
    // search the node
    offset_t offset = search(id, true);

    if(offset != INVALID_OFFSET) { // found it
        nodeInfo_t *nodeInfo = _history.takeFirst();

        _updateNodes.push_back(nodeInfo);
        // position
        int i = nodeInfo->idx;

        int nodeInfoIdx = -1; // this shall keep the index of nodeInfo in _history, we have to delete later
        rrn_t nodeInfoRRN = INVALID_RRN; // same
        if(nodeInfo->node.links[i] != INVALID_RRN || nodeInfo->node.links[i+1] != INVALID_RRN) { // non-leaf
            nodeInfo_t *leafNode = new nodeInfo_t;

            nodeInfoIdx = _history.size();
            nodeInfoRRN = nodeInfo->rrn;
            // WELL, history isn't always fair...
            _history.push_front(nodeInfo); // nodeInfo is, after all, history

            if(nodeInfo->node.links[i] != INVALID_RRN) { // Let's get the greater key from the left subtree!!!
                getNodeAt(leafNode->node, nodeInfo->node.links[i]); // get the left subtree
                leafNode->rrn = nodeInfo->node.links[i]; // set it's rrn
                
                while(leafNode->node.links[leafNode->node.keyNumber] != INVALID_RRN) {
                    leafNode->idx = leafNode->node.keyNumber;
                    _history.push_front(leafNode);

                    nodeInfo_t *aux = leafNode;

                    leafNode = new nodeInfo_t;
                    // keep going to the most right subtree
                    getNodeAt(leafNode->node, aux->node.links[aux->node.keyNumber]);
                    leafNode->rrn = aux->node.links[aux->node.keyNumber];
                }

                leafNode->idx = -1;

                // swap the node that we want to remove and this from the leaf
                nodeInfo->node.keys[i] = leafNode->node.keys[leafNode->node.keyNumber-1];

                log().hold(true);
                log() << "Chave " << nodeInfo->node.keys[i].value << " promovida.\n";
                log() << "Chave " << id << " rebaixada.\n";
                log().hold(false);

                // we dont loose the reference: everything is inside _history
                nodeInfo = leafNode; // update the node that we have to work
                i = leafNode->node.keyNumber-1; // and the index of the key

                removeKeyFromNode(nodeInfo->node, i, RIGHT_SIDE);
            } else if(nodeInfo->node.links[i+1] != INVALID_RRN) { // Welll, get the smmaler key from the right subtree
                getNodeAt(leafNode->node, nodeInfo->node.links[i+1]);
                leafNode->rrn = nodeInfo->node.links[i+1];

                while(leafNode->node.links[0] != INVALID_RRN) {
                    leafNode->idx = 0;
                    _history.push_front(leafNode);

                    nodeInfo_t *aux = leafNode;

                    leafNode = new nodeInfo_t;
                    getNodeAt(leafNode->node, aux->node.links[0]);
                    leafNode->rrn = aux->node.links[0];
                }

                leafNode->idx = -1;

                // swap the nodes
                nodeInfo->node.keys[i] = leafNode->node.keys[0];

                log().hold(true);
                log() << "Chave " << nodeInfo->node.keys[i].value << " promovida.\n";
                log() << "Chave " << id << " rebaixada.\n";
                log().hold(false);

                // we dont loose the reference: everything is inside _history
                nodeInfo = leafNode; // update the node that we have to work
                i = 0; // and the index

                removeKeyFromNode(nodeInfo->node, i, LEFT_SIDE);
            }

            _updateNodes.push_back(leafNode);

        } else { // leaf
            removeKeyFromNode(nodeInfo->node, i, LEFT_SIDE);
        }


        bool updateNode = true;

        while(updateNode && _history.isEmpty() == false) {
            if(nodeInfo->rrn == rootRRN()) {
                updateNode = false;

                if(nodeInfo->node.keyNumber == 0) {
                    setRootRRN(INVALID_RRN);
                }
            } else {
                if(nodeInfo->node.keyNumber < BTREE_MIN_KEY_NUM) { // shit, we have underflow
                    nodeInfo_t *fatherInfo = _history.takeFirst();
                    int idx = fatherInfo->idx;

                    nodeInfo_t *leftSibling = NULL;
                    if(idx > 0) {
                        leftSibling = new nodeInfo_t;
                        getNodeAt(leftSibling->node, fatherInfo->node.links[idx - 1]);
                        leftSibling->idx = -1;
                        leftSibling->rrn = fatherInfo->node.links[idx - 1];
                    }

                    nodeInfo_t *rightSibling = NULL;
                    if(idx < fatherInfo->node.keyNumber) {
                        if(leftSibling == NULL || leftSibling->node.keyNumber <= BTREE_MIN_KEY_NUM) {
                            rightSibling = new nodeInfo_t;
                            getNodeAt(rightSibling->node, fatherInfo->node.links[idx + 1]);
                            rightSibling->idx = -1;
                            rightSibling->rrn = fatherInfo->node.links[idx + 1];
                        }
                    }

                    if(rightSibling != NULL) {
                        if(rightSibling->node.keyNumber > BTREE_MIN_KEY_NUM) {
                            appendKeyToNode(nodeInfo->node, fatherInfo->node.keys[idx], rightSibling->node.links[0]);

                            fatherInfo->node.keys[idx] = rightSibling->node.keys[0];

                            removeKeyFromNode(rightSibling->node, 0, LEFT_SIDE);
                            updateNode = false;

                            log() << "Redistribuicao de chaves - entre as paginas irmas X e Y.\n";
                        } else {
                            appendKeyToNode(nodeInfo->node, fatherInfo->node.keys[idx], rightSibling->node.links[0]);
                            removeKeyFromNode(fatherInfo->node, idx, RIGHT_SIDE);
                            rightSibling->node.links[0] = INVALID_RRN;

                            int i;
                            for(i = 0; rightSibling->node.keyNumber > 0; i++) {
                                appendKeyToNode(nodeInfo->node, rightSibling->node.keys[i], rightSibling->node.links[i+1]);

                                rightSibling->node.keys[i].value = DEFAULT_KEY;
                                rightSibling->node.keys[i].offset = INVALID_OFFSET;
                                rightSibling->node.links[i+1] = INVALID_RRN;
                                rightSibling->node.keyNumber--;
                            }

                            log() << "Concatenacao de chaves - entre as paginas irmas X e Y.\n";
                        }

                        _updateNodes.push_back(rightSibling);

                        if(leftSibling != NULL) {
                            delete leftSibling;
                        }
                    } else if(leftSibling != NULL) {
                        if(leftSibling->node.keyNumber > BTREE_MIN_KEY_NUM) {
                            prependKeyToNode(nodeInfo->node, fatherInfo->node.keys[idx-1], leftSibling->node.links[leftSibling->node.keyNumber]);

                            fatherInfo->node.keys[idx-1] = leftSibling->node.keys[leftSibling->node.keyNumber-1];

                            removeKeyFromNode(leftSibling->node, leftSibling->node.keyNumber-1, RIGHT_SIDE);
                            updateNode = false;

                            log() << "Redistribuicao de chaves - entre as paginas irmas X e Y.\n";
                        } else {
                            appendKeyToNode(leftSibling->node, fatherInfo->node.keys[idx-1], nodeInfo->node.links[0]);
                            removeKeyFromNode(fatherInfo->node, idx-1, RIGHT_SIDE);
                            nodeInfo->node.links[0] = INVALID_RRN;

                            int i;
                            for(i = 0; nodeInfo->node.keyNumber > 0; i++) {
                                appendKeyToNode(leftSibling->node, nodeInfo->node.keys[i], nodeInfo->node.links[i+1]);

                                nodeInfo->node.keys[i].value = DEFAULT_KEY;
                                nodeInfo->node.keys[i].offset = INVALID_OFFSET;
                                nodeInfo->node.links[i+1] = INVALID_RRN;
                                nodeInfo->node.keyNumber--;
                            }

                            log() << "Concatenacao de chaves - entre as paginas irmas X e Y.\n";
                        }

                        _updateNodes.push_back(leftSibling);

                        if(rightSibling != NULL) {
                            delete rightSibling;
                        }
                    } else {
                        closeIndexFile();
                        std::cout << "[Warning] Couldn't find siblings.\n";
                        return INVALID_OFFSET;
                    }

                    if(fatherInfo->rrn != nodeInfoRRN) { // make sure that we dont put two copys to be updated
                        _updateNodes.push_back(fatherInfo);
                    }
                    nodeInfo = fatherInfo;
                }
            }
        }

        if(updateNode) {
            if(nodeInfo->rrn == rootRRN()) {
                if(nodeInfo->node.keyNumber == 0) {
                    setRootRRN(nodeInfo->node.links[0]);
                }
            }
        }

        if(nodeInfoIdx >= 0) {
            _history.remove(nodeInfoIdx);
        }
    } else { // INVALID_OFFSET
        log().hold(true);
        log() << "Chave " << id << " nao cadastrada.\n";
        log().hold(false);

        closeIndexFile();
        return INVALID_OFFSET;
    }

    if( applyUpdates() != 0) {

        closeIndexFile();
        return INVALID_OFFSET;
    }

    closeIndexFile();

    log().hold(true);
    log() << "Chave " << id << " removida com sucesso.\n";
    log().hold(false);

    return offset;
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

            nodeInfo_t *nodeInfo = NULL; // what a wast...
            if(makeHistory) {
                nodeInfo = new nodeInfo_t; // this solve the problem if the node is too big
                getNode(nodeInfo->node); // put the node into nodeInfo->node
                node = &(nodeInfo->node); // we can do this because we won't delete this nodeInfo until the end
                nodeInfo->rrn = currRRN;

                _history.push_front(nodeInfo); // add to _history
            } else {
                node = &loadNode(); // we can do this because the function return a reference to _node
            }

            // search key inside node
            int i = 0;
            while(i < node->keyNumber && key > node->keys[i].value) {
                i++;
            }

            if(node->keys[i].value == key) {
                found = true;
                result = node->keys[i].offset;
            }

            if(makeHistory) {
                nodeInfo->idx = i;
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

void BTree::print() {
    log() << "Execucao de operacao para mostrar a arvore-B gerada:\n";

    if(rootRRN() == INVALID_RRN) {
        return;
    }

    openIndexFile();

    _printQueue.clean();
    _printQueue.push_back(rootRRN());
    _printQueue.push_back(INVALID_RRN);

    int level = 0;
    while(_printQueue.isEmpty() == false) {
        rrn_t curr = _printQueue.takeFirst();
        if(curr == INVALID_RRN) {
            level++;

            if(_printQueue.isEmpty() == false) {
                _printQueue.push_back(INVALID_RRN);
            }

            continue;
        }

        log().hold(true);
        log() << level << " ";

        Node &node = loadNodeAt(curr);
        printNode(node);
        log().hold(false);

        int i = 0;
        while(i <= node.keyNumber && node.links[i] != INVALID_RRN) {
            _printQueue.push_back(node.links[i]);
            i++;
        }
    }

    closeIndexFile();
}


void BTree::printNode(Node &node) {
    log().hold(true);
    log() << node.keyNumber << " ";
    int i;
    for(i = 0; i < node.keyNumber; i++) {
        log() << "<" << node.keys[i].value << "/" << node.keys[i].offset << "> ";
    }


    log() << "\n";
    log().hold(false);
}

int BTree::applyUpdates() {
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

    _history.deleteAll();
    _updateNodes.deleteAll();
    return 0;
}
