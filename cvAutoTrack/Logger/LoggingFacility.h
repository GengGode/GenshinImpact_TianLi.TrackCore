#pragma once

#include <string>
#include <memory>

class LoggingFacility {
public:
    //virtual ~LoggingFacility() = default;
    virtual void Log(std::string message) = 0;
    virtual void Info(std::string message) = 0;
    virtual void Warning(std::string message) = 0;
    virtual void Error(std::string message) = 0;
};

using Logger = std::shared_ptr<LoggingFacility>;