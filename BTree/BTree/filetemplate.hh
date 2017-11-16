#ifndef FILETEMPLATE_H
#define FILETEMPLATE_H

#include <iostream>

/* This class shall encapsulate the template of
 * the data file
 */
class FileTemplate {
private:
    char *_filePath;
public:
    FileTemplate(char *filePath);

    virtual int getNextData() = 0;
    virtual std::string operator[](std::size_t idx) = 0;

    /* number of elements in a data set */
    virtual int dataSize() = 0;
    /* number of data sets in a file */
    virtual int fileSize() = 0;

    virtual int createIndexFile() = 0;
};

#endif // FILETEMPLATE_H
