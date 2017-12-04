#ifndef REGISTERPARSER_HH
#define REGISTERPARSER_HH

#include "sharedheader.hh"
#include "loghandle.hh"

using namespace std;

class RegisterParser {
friend class DataHandle;
public:
    struct Register_t {
        int id;
        char titulo[TITLE_LENGTH];
        char genero[GENDER_LENGTH];
    };

private:
    struct Header {
        static constexpr char headerMsg[] = {'D', 'A', 'T'};
        offset_t lastModifiedOffset;
    };

    Header _header;
    bool _isHeaderUpdated;
    void writeHeader();
    void readHeader();
    offset_t lastModifiedOffset();
    void setLastModifiedOffset(offset_t offset);

    char *_registerBuffer;
    Register_t *_register;
    bufferptr_t _bufferSize;

    const char *_dataFile;
    fstream _dataStream;
    int _streamCounter;
    bool _hold;

    LogHandle *_log;
public:
    RegisterParser(const char *dataFile, LogHandle *_log);
    ~RegisterParser();

    LogHandle &log() { return *_log; }

    const char *dataPath() { return _dataFile; }

    bool openDataFile();
    bool closeDataFile();
    void hold(bool hold) { _hold = hold; }

    const char *registerParser() { return _registerBuffer; }
    const char *registerParser(Register_t reg);
    Register_t &registerParser(const char *reg);

    int stringCopy(char *dest, const char *src, int maxLength);

    void removeRegisterAt(offset_t offset);
    const char *getRegisterAt(offset_t offset);
    Register_t &decodeRegisterAt(offset_t offset);

    const char *getNextRegister();
    Register_t &decodeNextRegister();

    Register_t &decodeLastModifiedRegister();

    offset_t readOffset();
    offset_t writeOffset();
    void reset();

    offset_t pushRegister(Register_t &reg);
};

#endif // REGISTERPARSER_HH
