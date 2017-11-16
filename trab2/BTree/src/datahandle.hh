#ifndef FILETEMPLATE_H
#define FILETEMPLATE_H

#include <iostream>

class DataHandle {
private:
    char *_dataPath;
public:
    DataHandle(char *dataPath);

    virtual int getNextRegister() = 0;
    virtual std::string getRegisterByOffset(int offset) = 0;

    /*Retorna o tamanho de um registro*/
    virtual int registerSize() = 0;
    /* Retorna o número de registro no arquivo de índices*/
    virtual int registerNumber() = 0;

    virtual int createIndexFile() = 0;

    virtual int addRegister() = 0;

    virtual int removeRegister() = 0;
};

#endif // FILETEMPLATE_H
