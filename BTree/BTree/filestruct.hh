#ifndef FILESTRUCT_H
#define FILESTRUCT_H

#include "filetemplate.hh"

class FileStruct : public FileTemplate {
private:
    typedef struct {
        int id;
        char titulo[30];
        char genero[20];
    } tRegistro;

    tRegistro _reg;
public:
    FileStruct(char *filePath);

    int getNextData();
    int dataSize();
    int fileSize();
    int createIndexFile();

    std::string operator[](std::size_t idx);
};

#endif // FILESTRUCT_H
