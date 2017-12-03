#ifndef LOGHANDLE_HH
#define LOGHANDLE_HH

#include <iostream>
#include <fstream>

class LogHandle {
private:
    const char *_logFile;
    std::fstream _logStream;

    bool _hold;

    bool openLog();
    void closeLog();
public:
    LogHandle(const char *logFile);
    ~LogHandle();

    void hold(bool hold);
    void saveOnLog(const char *log);

    LogHandle &operator <<(const char *msg);
    LogHandle &operator <<(int msg);
    LogHandle &operator <<(long msg);
};

#endif // LOGHANDLE_HH
