//
// Created by GengG on 2022/1/27.
//

#ifndef CVAUTOTRACKDLL_TASKSLOGGER_H
#define CVAUTOTRACKDLL_TASKSLOGGER_H

#include "StandardOutputLogger.h"

class TasksLogger: public StandardOutputLogger
{
public:
    TasksLogger()
    {
        this->_objectMessage = "[ Task ]";
    }
    void setLogLevel(LogLevel level) override
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
#endif //CVAUTOTRACKDLL_TASKSLOGGER_H
