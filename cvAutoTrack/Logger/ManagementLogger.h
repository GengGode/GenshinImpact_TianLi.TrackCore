//
// Created by GengG on 2022/1/27.
//

#ifndef CVAUTOTRACKDLL_MANAGEMENTLOGGER_H
#define CVAUTOTRACKDLL_MANAGEMENTLOGGER_H

#include "StandardOutputLogger.h"

class ManagementLogger: public StandardOutputLogger
{
public:
    ManagementLogger()
    {
        this->_objectMessage = "[ Management ]";
    }
    void setLogLevel(LogLevel level)
    {
        this->_logLevel = level;
    }
    void log(std::string message)
    {
        StandardOutputLogger::log(message);
    }
    void info(std::string message)
    {
        StandardOutputLogger::info(message);
    }
    void warning(std::string message)
    {
        StandardOutputLogger::warning(message);
    }
    void error(std::string message)
    {
        StandardOutputLogger::error(message);
    }
};
#endif //CVAUTOTRACKDLL_MANAGEMENTLOGGER_H
