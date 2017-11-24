#ifndef DATAPARSER_HH
#define DATAPARSER_HH

#include "commonheader.h"

using namespace std;

class DataParser {
private:
    const char *_dataFile;
    fstream _dataStream;
public:
    DataParser(const char *dataFile);
    ~DataParser();
};

#endif // DATAPARSER_HH
