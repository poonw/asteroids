#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

class Logger
{
public:
    enum
    {
        ALL = 0,
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        NONE
    };

    static Logger& getInstance(void);

    void setVerbosityLevel(int level);
    void setLogFile(const std::string& logFilePath);
    void log(int level, const std::string& message);

private:
    Logger(void);
    ~Logger(void);
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

    int           m_verbosityLevel = Logger::ALL;
    std::string   m_logFilePath    = "";
    std::ofstream m_logFile;
    std::mutex    m_mutex;
};

#endif // LOGGER_H
