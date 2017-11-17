#include "dataparser.hh"

DataParser::DataParser(const char *path, RegisterHandle *registerHandle) : DataHandle(path, registerHandle) {
}

int DataParser::getNextRegister() {
    return 0;
}

RegisterHandle &DataParser::getRegisterByOffset(int offset) {
    ifstream file;
    file.open(_dataPath, ios::binary);

    if(file.rdstate()) {
        file.seekg(offset, ios_base::beg);

        if(file.rdstate()) {
            cout << "An error occur when seeking the requested offset.\n";
        } else {
            int size;
            file >> size;

            char *reg = new char[size + 1];
            file.read(reg, size);

            reg[size + 1] = '\0';

            string regString(reg);

            delete reg;

            _registerHandle->setRegister(regString);
        }

        file.close();
    } else {
        _registerHandle->setRegister("");
    }
    return *_registerHandle;
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
