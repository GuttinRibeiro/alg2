#ifndef DATAHANDLE_HH
#define DATAHANDLE_HH

#include "sharedheader.h"
#include "btree.hh"
#include "registerparser.hh"
#include "loghandle.hh"

class DataHandle {
private:
    BTree *_btree;
    RegisterParser *_parser;
    LogHandle *_log;
public:
    DataHandle(const char *dataFile, const char *indexFile, const char *logFile);
    ~DataHandle();

    void insert(RegisterParser::Register_t &reg);
    RegisterParser::Register_t search(int id);

    void rebuildIndexFile();

    void printBTree();

    LogHandle &log() { return *_log; }
};

#endif // DATAHANDLE_HH
