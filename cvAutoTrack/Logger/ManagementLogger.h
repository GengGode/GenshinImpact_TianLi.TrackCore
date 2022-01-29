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
#endif //CVAUTOTRACKDLL_MANAGEMENTLOGGER_H
