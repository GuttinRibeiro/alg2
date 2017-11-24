#ifndef REGISTERPARSER_HH
#define REGISTERPARSER_HH

#include "commonheader.h"

using namespace std;

class RegisterParser {
public:
    struct Register_t {
        int id;
        char titulo[TITLE_LENGTH];
        char genero[GENDER_LENGTH];
    };

private:
    char _registerBuffer[BUFFER_SIZE];
    Register_t _register;

public:
    RegisterParser();

    const char *registerParser() { return _registerBuffer; }
    const char *registerParser(Register_t reg);
    Register_t &registerParser(const char *reg);

    int stringCopy(char *dest, const char *src, int maxLength);
};

#endif // REGISTERPARSER_HH
