#include "dataparser.hh"

DataParser::DataParser(char *path) : DataHandle(path) {
}

int DataParser::getNextRegister() {
    return 0;
}

std::string DataParser::getRegisterByOffset(int offset) {
    return "";
}

int DataParser::registerNumber() {
    return 0;
}

int DataParser::createIndexFile() {
    return 0;
}

int DataParser::addRegister() {
    return 0;
}

int DataParser::removeRegister() {
    return 0;
}
