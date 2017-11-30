#include "registerparser.hh"

constexpr char RegisterParser::Header::headerMsg[];

RegisterParser::RegisterParser(const char *dataFile, LogHandle *log) {
    _log = log;

    _dataFile = dataFile;
    _hold = false;

    _dataStream.open(_dataFile, fstream::out // open as write
                              | fstream::in  // and read file
                              | fstream::binary); // open as binary file
    // test file
    if(_dataStream.is_open()) {
        // test if the file is empty
        if(_dataStream.peek() == ifstream::traits_type::eof()) {
            cerr << "[Warning] Register receive an empty file. Overwriting.\n";

            // for some reason, the file doesn't write with in and out
            _dataStream.close();
            _dataStream.open(dataFile, fstream::out // open as write
                                     | fstream::binary); // set as a binary file

            // test if we can write on the file
            if(!(_dataStream.write(Header::headerMsg, 3) )) {
                cerr << "[Error] Register couldn't write on file.\n";
                abort();
            }
            _dataStream.close();
        } else {
            // file isn't empty

            // test if we can read the file
            char headerMsg[3];
            if( !(_dataStream.read(headerMsg, 3)) ) {
                cerr << "[Error] Register couldn't read the file.\n";
                abort();
            }

            // test if we can write on the file
            _dataStream.seekp(0);
            if(!(_dataStream.write(headerMsg, 3) )) {
                cerr << "[Error] Register couldn't write on file.\n";
                abort();
            }

            // test if the file was created by us
            if(strncmp(headerMsg, Header::headerMsg, 3) != 0) {
                cerr << "[Warning] Register receive an corrupted file.\n";
            }
        }

    } else { // file dosn't exist
        cerr << "[Warning] Register coudn't open index file, creating a new one at \"" << dataFile << "\".\n";
        _dataStream.open(_dataFile, fstream::out // open as write
                                  | fstream::binary); // set as a binary file

        // test if we can write on the file
        if(!(_dataStream.write(Header::headerMsg, 3) )) {
            cerr << "[Error] Register couldn't write on file.\n";
            abort();
        }
    }

    _dataStream.close();
    _streamCounter = 0;

//    _registerBuffer = new char[BUFFER_SIZE];
    _register = new Register_t;

    _bufferSize = 0;
}

RegisterParser::~RegisterParser() {
    if(_dataStream.is_open()) {
        _dataStream.close();
    }
}

bool RegisterParser::openDataFile() {
    if(_hold == false || _dataStream.is_open() == false) {
        if(_dataStream.is_open() == false) {
            _dataStream.open(_dataFile, fstream::out // open as write
                                      | fstream::in  // and read file
                                      | fstream::binary // open as binary file
                                      | fstream::ate); // set the write pointer at the end of file
//                                      | fstream::app); // let all the writes to be at the end of file

            _streamCounter = 1;
        } else {
            _streamCounter++;
        }
    }

    return _dataStream.is_open();
}

void RegisterParser::closeDataFile() {
    if(_hold == false) {
        // close the file only if the function was the one to open it
        if(_streamCounter == 1) {
            _dataStream.close();
        }

        _streamCounter--;
    }
}

const char *RegisterParser::registerParser(Register_t reg) {
    // the buffer will be [size of register]|[id]|[title]|[gender]
    // bufferPointer is here ~~~~~~~~~~~~~~~^
    bufferptr_t bufferPointer = sizeof(bufferPointer);

    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put id into buffer
    memcpy(&(_registerBuffer[bufferPointer]), &(reg.id), sizeof(reg.id));
    bufferPointer += sizeof(reg.id);
    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put title into buffer
    bufferPointer += stringCopy(&(_registerBuffer[bufferPointer]), reg.titulo, TITLE_LENGTH);
    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put gender into buffer
    bufferPointer += stringCopy(&(_registerBuffer[bufferPointer]), reg.genero, GENDER_LENGTH);
    // delimiter
    _registerBuffer[bufferPointer++] = '|';

    // put the size of the register into buffer
    memcpy(_registerBuffer, &(bufferPointer), sizeof(bufferPointer));

    // set the current buffer size
    _bufferSize = bufferPointer;

    return _registerBuffer;
}

RegisterParser::Register_t &RegisterParser::registerParser(const char *reg) {
    bufferptr_t size = 0;
    memcpy(&size, reg, sizeof(size));

    bufferptr_t bufferPointer = sizeof(size) + 1;

    if(size > bufferPointer) {
        memcpy(&(_register->id), &(reg[bufferPointer]), sizeof(_register->id));
        bufferPointer += sizeof(_register->id) + 1;
    }

    if(size > bufferPointer) {
        bufferPointer += stringCopy(_register->titulo, &(reg[bufferPointer]), TITLE_LENGTH);
        bufferPointer += 1; // delimiter
    }

    if(size > bufferPointer) {
        bufferPointer += stringCopy(_register->genero, &(reg[bufferPointer]), GENDER_LENGTH);
        bufferPointer += 1; // delimiter
    }

    return *_register;
}

int RegisterParser::stringCopy(char *dest, const char *src, int maxLength) {
    int i = 0;
    while(src[i] != '\0' && i < maxLength) {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
    return (++i);
}

void RegisterParser::removeRegisterAt(offset_t offset) {
    if(openDataFile()) {
        _dataStream.seekp(offset + sizeof(bufferptr_t) + 1); // remember: delimiter
        int invalidId = DEFAULT_KEY;
        _dataStream.write((char *)&invalidId, sizeof(invalidId));
    }

    closeDataFile();
}

const char *RegisterParser::getRegisterAt(offset_t offset) {
    if(openDataFile()) {
        _dataStream.seekg(offset);

        getNextRegister();
    }

    closeDataFile();
    return _registerBuffer;
}

RegisterParser::Register_t &RegisterParser::decodeRegisterAt(offset_t offset) {
    return registerParser(getRegisterAt(offset));
}

offset_t RegisterParser::pushRegister(Register_t &reg) {
    offset_t offset = INVALID_OFFSET;
    if(openDataFile()) {
        offset = _dataStream.tellp();
        const char *buff = registerParser(reg);
        _dataStream.write(buff, _bufferSize);
    }

    closeDataFile();

    return offset;
}

const char *RegisterParser::getNextRegister() {
    if(openDataFile()) {
        bufferptr_t size = 0;
        // read the size of the register;
        _dataStream.read(_registerBuffer, sizeof(size));
        memcpy(&size, _registerBuffer, sizeof(size));

        // read the rest
        _dataStream.read(&(_registerBuffer[sizeof(size)]), size - sizeof(size));

        _bufferSize = size;
    }

    closeDataFile();

    return _registerBuffer;
}

RegisterParser::Register_t &RegisterParser::decodeNextRegister() {
    return registerParser(getNextRegister());
}

offset_t RegisterParser::readOffset() {
    if(_dataStream.is_open() && _dataStream.peek() != ifstream::traits_type::eof()) {
        return _dataStream.tellg();
    } else {
        return INVALID_OFFSET;
    }
}

offset_t RegisterParser::writeOffset() {
    if(_dataStream.is_open()) {
        _dataStream.seekp(0, ios::end);
        return _dataStream.tellp();
    } else {
        return INVALID_OFFSET;
    }
}

void RegisterParser::reset() {
    _dataStream.seekg(3);
}
