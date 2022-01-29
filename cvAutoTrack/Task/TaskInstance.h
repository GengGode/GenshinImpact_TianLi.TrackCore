//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_TASKINSTANCE_H
#define CVAUTOTRACKDLL_TASKINSTANCE_H

#include "TaskFacility.h"
#include "../Logger/LoggingFacility.h"
#include "../Logger/TasksLogger.h"
#include <any>
#include <thread>
#include <functional>

class TaskInstance:public TaskFacility
{

public:
    Logger log;
    std::thread* _thread= nullptr;
    std::atomic<bool> _isRunService;
    std::atomic<bool> _isStartService;
    std::atomic<bool> _isWorkEnd;

    std::any _value_output;

    std::function<void()> _task;
    TaskInstance()=delete;
    TaskInstance(const Logger& logService)
    {
        log= logService;
        _task=[=]{log->Warning("Void Task Function");};
    }
    void beginTask()
    {
        log->Info("begin task");
        _isRunService=true;

        try{
            _thread=new std::thread(&TaskInstance::run,this);
            log->Log("Thread Create Success");
        }
        catch (std::exception &e)
        {
           log->Error("Thread Create Error"+std::string(e.what()));
        }
    }
    void run()
    {
        log->Log("begin thread");
        // 任务循环是否继续
        while(_isRunService)
        {
            // 是否执行任务
            if(_isStartService)
            {
                //log->Log("service start");
                _isWorkEnd=false;
                _task();
                _isWorkEnd=true;
            }
            else
            {
                //log->Log("service stop");
                std::this_thread::sleep_for(std::chrono::microseconds (1));
            }
        }
        log->Log("end thread");
    }
    void endTask()
    {
        _isRunService=false;
        _isStartService=false;
        if(_thread!= nullptr)_thread->join();
        delete _thread;
        log->Info("end task");
    }
    bool isWorkEnd()
    {
        return _isWorkEnd;
    }
    void setStartWork()
    {
        _isStartService=true;
    }
    void setStopWork()
    {
        _isStartService=false;
    }

    void setOutputValue(std::any value)
    {
        _value_output=value;
    }
    std::any getOutputValue()
    {
        return _value_output;
    }
};


#endif //CVAUTOTRACKDLL_TASKINSTANCE_H
