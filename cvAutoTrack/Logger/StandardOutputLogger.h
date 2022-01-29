#pragma once
#include "LoggingFacility.h"
#include "../Timer/SystemTime.h"

#include <thread>
#include <iostream>
#include <queue>

class StandardOutputLogger:public LoggingFacility 
{
    std::thread* _threadInstance= nullptr;
    std::queue<std::string> _logQueue;
    std::atomic<bool> _isRunning;
public:
    std::string _coutNowTimeInfo;
    std::string _objectMessage;
    SystemTimer _systemTimer;

    std::ostream& ioOutput = std::cout;

	StandardOutputLogger(){
        _isRunning=true;
        _threadInstance=new std::thread(&StandardOutputLogger::listen,this);
        Log("标准输出日志系统启动");
    }

    ~StandardOutputLogger()
    {
        Log("标准输出日志系统关闭");

        if(!_logQueue.empty())
        {
            for (int i = 0; i < _logQueue.size(); ++i) {
                std::string message = _logQueue. front();
                _logQueue.pop();
                ioOutput << message << "\n";
            }
        }

        _isRunning= false;
        if(_threadInstance!= nullptr)_threadInstance->join();
        delete _threadInstance;


    }

    virtual void Log(std::string message)
    {
        push("[   log   ]"+_objectMessage+":" +message);
    }
    virtual void Info(std::string message)
    {
        push("[  info   ]"+_objectMessage+":"+message);
    }
    virtual void Warning(std::string message)
    {
        push("[ warning ]"+_objectMessage+":"+message);
    }

    virtual void Error(std::string message )
    {
        push("[  error  ]" +_objectMessage+":"+ message) ;
    }

    std::string getNowTime()
    {
        _coutNowTimeInfo = _systemTimer.now();
        return "[" + _coutNowTimeInfo + "]";
    }

protected:
    void push(std::string message)
    {
        append(message);
    }

private:
    void append(std::string message)
    {
        _logQueue.push( getNowTime()  + message);
    }


    void listen()
    {
        while (_isRunning)
        {
            if(!_logQueue.empty())
            {
                for (int i = 0; i < _logQueue.size(); ++i) {
                    std::string message = _logQueue. front();
                    _logQueue.pop();
                    ioOutput << message << "\n";
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
};