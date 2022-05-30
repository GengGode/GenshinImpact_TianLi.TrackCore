#pragma once

#include <string>
#include <memory>
enum LogLevel
{
    Log,
    Info,
    Warning,
    Error,
    Release
};

class LoggingFacility {
public:
    LogLevel _logLevel=Log;
    
    //virtual ~LoggingFacility() = default;
    virtual void setLogLevel(LogLevel level) = 0;
    virtual void log(std::string message) = 0;
    virtual void info(std::string message) = 0;
    virtual void warning(std::string message) = 0;
    virtual void error(std::string message) = 0;
};

using Logger = std::shared_ptr<LoggingFacility>;