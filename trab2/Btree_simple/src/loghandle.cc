#include "loghandle.hh"

LogHandle::LogHandle(const char *logFile) {
    _logFile = logFile;
}

void LogHandle::hold(bool hold) {
    if(hold == false) {
        _logStream.close();
    }

    _hold = hold;
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
    if(_hold == false) {
        _logStream.close();
    }
}

LogHandle &LogHandle::operator <<(const char *msg) {
    if(openLog()) {
        _logStream << msg;
        std::cout << msg;
    }

    closeLog();

    return *this;
}

LogHandle &LogHandle::operator <<(int msg) {
    if(openLog()) {
        _logStream << msg;
        std::cout << msg;
    }

    closeLog();;

    return *this;
}

LogHandle &LogHandle::operator <<(long msg) {
    if(openLog()) {
        _logStream << msg;
        std::cout << msg;
    }

    closeLog();;

    return *this;
}
