#ifndef REGISTERPARSER_H
#define REGISTERPARSER_H

#include "registerhandle.hh"

using namespace std;

class RegisterParser : public RegisterHandle {
private:
    enum {
        ID_FIELD = 0,
        NAME_FIELD = 1,
        _FIELD = 2
    };
public:
    RegisterParser(string reg);

    void setRegister(string reg) { _reg = reg; _parsed = false; }
    vector<string> RegisterFieldParser();
    int getKey();

    string operator [](size_t idx);
};

#endif // REGISTERPARSER_H
