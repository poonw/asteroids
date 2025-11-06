#include "Logger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

Logger& Logger::getInstance(void)
{
    static Logger instance;
    return instance;
}

Logger::Logger(void)
{
}

Logger::~Logger(void)
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void Logger::setVerbosityLevel(int level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_verbosityLevel = level;
}

void Logger::setLogFile(const std::string& logFilePath)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logFilePath = logFilePath;

    if (m_logFilePath.empty())
    {
        std::cerr << "\033[31mEmpty logFilePath\033[0m" << std::endl;
    }
    else
    {
        // Open the log file if it's not already open
        if (!m_logFile.is_open())
        {
            m_logFile.open(m_logFilePath, std::ios_base::app);
            if (!m_logFile.is_open())
            {
                std::cerr << "\033[31mFailed to open log file: " << m_logFilePath << "\033[0m" << std::endl;
            }
        }
    }
}

// Log a message with the specified verbosity level
void Logger::log(int level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (level >= m_verbosityLevel)
    {
        auto               now  = std::chrono::system_clock::now();
        auto               time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;

#ifdef _WIN32
        std::tm tm_buf;
        if (localtime_s(&tm_buf, &time) != 0)
        {
            oss << "[ERROR: Failed to get local time] ";
        }
        else
        {
            oss << "[" << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S") << "] ";
        }
#else
        std::tm* tm_ptr = std::localtime(&time);
        if (!tm_ptr)
        {
            oss << "[ERROR: Failed to get local time] ";
        }
        else
        {
            oss << "[" << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S") << "] ";
        }
#endif

        // Format the log message
        std::string logMessage = "";
        switch (level)
        {
            case Logger::DEBUG:
                logMessage += "[DEBUG] ";
                break;
            case Logger::INFO:
                logMessage += "[INFO] ";
                break;
            case Logger::WARNING:
                logMessage += "\033[33m[WARNING] ";
                break;
            case Logger::ERROR:
                logMessage += "\033[31m[ERROR] ";
                break;
            case Logger::FATAL:
                logMessage += "\033[31m[FATAL] ";
                break;
        }
        logMessage += oss.str().substr(0, 24) + " " + message + "\033[0m\n";

        // Print the log message to the console
        std::cout << logMessage;

        // Write the log message to the file if it's open
        if (m_logFile.is_open())
        {
            m_logFile << logMessage;
            m_logFile.flush();
        }
    }
}
