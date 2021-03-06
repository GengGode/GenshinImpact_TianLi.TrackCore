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
    std::any _value_input;

    std::function<void()> _task;
    TaskInstance()=delete;
    TaskInstance(const Logger& logService)
    {
        log= logService;
        _task=[=]{ log->warning("Void Task Function");};
    }
    void beginTask()
    {
        log->info("begin task");
        _isRunService=true;

        try{
            _thread=new std::thread(&TaskInstance::run,this);
            log->log("Thread Create Success");
        }
        catch (std::exception &e)
        {
            log->error("Thread Create error" + std::string(e.what()));
        }
    }
    void run()
    {
        log->log("begin thread");
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

                //TODO: 线程休眠
                std::this_thread::sleep_for(std::chrono::microseconds (10));
            }
        }
        log->log("end thread");
    }
    void endTask()
    {
        _isRunService=false;
        _isStartService=false;
        if(_thread!= nullptr)_thread->join();
        delete _thread;
        log->info("end task");
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
    void setInputValue(std::any value)
    {
        _value_input=value;
    }
    void setOutputValue(std::any value)
    {
        _value_output=value;
    }
    std::any getInputValue()
    {
        return _value_input;
    }
    std::any getOutputValue()
    {
        return _value_output;
    }
};


#endif //CVAUTOTRACKDLL_TASKINSTANCE_H
