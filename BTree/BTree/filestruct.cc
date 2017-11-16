#include "filestruct.hh"

FileStruct::FileStruct(char *filePath) : FileTemplate(filePath) {

}


int FileStruct::getNextData() {
    return 0;
}

int FileStruct::dataSize() {
    return 0;
}

int FileStruct::fileSize() {
    return 0;
}

int FileStruct::createIndexFile() {
    return 0;
}

std::string FileStruct::operator[](std::size_t idx) {
    return "";
}
