#include "loghandle.hh"

LogHandle::LogHandle(const char *logFile) {
    _logFile = logFile;
    openLog();
}

LogHandle::~LogHandle() {
    _logStream.close();
}

bool LogHandle::openLog() {
    if(_logStream.is_open() == false) {
        _logStream.open(_logFile, std::fstream::out // open as an output file
                                | std::fstream::ate // output in the end of file
                                | std::fstream::app); // insert only in the end of file
    }

    return _logStream.is_open();
}

void LogHandle::closeLog() {
    _logStream.close();
}

LogHandle &LogHandle::operator <<(const char *msg) {
    _logStream << msg;
    std::cout << msg;

    return *this;
}

LogHandle &LogHandle::operator <<(int msg) {
    _logStream << msg;
    std::cout << msg;

    return *this;
}

LogHandle &LogHandle::operator <<(long msg) {
    _logStream << msg;
    std::cout << msg;

    return *this;
}
