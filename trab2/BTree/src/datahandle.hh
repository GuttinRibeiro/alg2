#ifndef FILETEMPLATE_H
#define FILETEMPLATE_H

#include <iostream>

class DataHandle {
private:
    char *_dataPath;
public:
    DataHandle(char *dataPath);

    virtual int getNextData() = 0;
    virtual std::string operator[](std::size_t idx) = 0;

    /* number of elements in a register */
    virtual int registerSize() = 0;
    /* number of registers in a file */
    virtual int registerNumber() = 0;

    virtual int createIndexFile() = 0;
};

#endif // FILETEMPLATE_H
