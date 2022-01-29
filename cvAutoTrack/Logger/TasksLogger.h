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
    void Log(std::string message)
    {
        StandardOutputLogger::Log(  message );
    }
    void Info(std::string message)
    {
        StandardOutputLogger::Info(  message );
    }
    void Warning(std::string message)
    {
        StandardOutputLogger::Warning(  message );
    }
    void Error(std::string message)
    {
        StandardOutputLogger::Error(  message );
    }
};
#endif //CVAUTOTRACKDLL_TASKSLOGGER_H
