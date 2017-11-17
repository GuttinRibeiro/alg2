#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include "datahandle.hh"
#include "registerhandle.hh"
#include <iostream>
#include <fstream>

class DataParser : public DataHandle {
public:
    DataParser(const char *path, RegisterHandle *registerHandle);

    int getNextRegister();
    RegisterHandle &getRegisterByOffset(int offset);

    int registerNumber();
    int createIndexFile();

    int addRegister();
    int removeRegister();
};

#endif // DATASTRUCT_H
