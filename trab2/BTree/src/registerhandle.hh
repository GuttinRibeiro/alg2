#ifndef REGISTERHANDLE_H
#define REGISTERHANDLE_H

#include <iostream>
#include <vector>

using namespace std;

#define INVALID_KEY -200

class RegisterHandle {
protected:
    string _reg;
    vector<string> _elements;
    int _key;
    bool _parsed;
public:
    RegisterHandle();
    RegisterHandle(string reg);

    virtual void setRegister(string reg) = 0;
    virtual vector<string> RegisterFieldParser() = 0;
    virtual int getKey() = 0;

    virtual string operator[](size_t idx) = 0;
};

#endif // REGISTERHANDLE_H
