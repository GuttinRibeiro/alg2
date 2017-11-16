#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include "datahandle.hh"
#include <iostream>

class DataParser : public DataHandle {
private:
    struct Register {

    };

    Register _reg;
public:
    DataParser(char *path);

    int getNextRegister();
    std::string getRegisterByOffset(int offset);

    int registerNumber();
    int createIndexFile();

    int addRegister();
    int removeRegister();
};

#endif // DATASTRUCT_H
