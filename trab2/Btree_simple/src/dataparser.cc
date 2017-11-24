#include "dataparser.hh"

DataParser::DataParser(const char *dataFile) {
    _dataFile = dataFile;
}

DataParser::~DataParser() {
    if(_dataStream.is_open()) {
        _dataStream.close();
    }
}
