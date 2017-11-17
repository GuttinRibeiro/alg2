#include "registerparser.hh"

RegisterParser::RegisterParser(string reg) : RegisterHandle(reg) {

}

vector<string> RegisterParser::RegisterFieldParser() {
    _parsed = true;

    vector<string> retn;
    string field;

    string::iterator it;
    for(it = _reg.begin(); it != _reg.end(); it++) {
        if(*it == '|') {
            retn.push_back(field);
            field.clear();
        } else {
            field.push_back(*it);
        }

    }

    _elements = retn;
    _key = stoi(retn.at(ID_FIELD));
    return retn;
}

int RegisterParser::getKey() {
    if(_parsed == false) {
        RegisterFieldParser();
    } else if(_elements.size() == 0) {
        return INVALID_KEY;
    }

    return _key;
}


string RegisterParser::operator [](size_t idx) {
    if(_parsed == false) {
        RegisterFieldParser();
    }

    if(idx < _elements.size()) {
        return _elements.at(idx);
    } else {
        return "";
    }
}
