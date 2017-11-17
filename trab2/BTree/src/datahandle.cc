#include "datahandle.hh"

DataHandle::DataHandle(const char *dataPath, RegisterHandle *registerHandle) {
    _dataPath = dataPath;
    _registerHandle = registerHandle;
}
