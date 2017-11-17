#include "registerhandle.hh"

RegisterHandle::RegisterHandle() {
    _reg = "";
    _key = INVALID_KEY;
    _parsed = false;
}

RegisterHandle::RegisterHandle(std::string reg) {
    _reg = reg;
    _key = INVALID_KEY;
    _parsed = false;
}
