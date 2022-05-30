#pragma once
#include "LoggingFacility.h"
#include "../Timer/SystemTime.h"

#include <thread>
#include <iostream>
#include <queue>
#include <mutex>

class StandardOutputLogger:public LoggingFacility 
{
    std::thread* _threadInstance= nullptr;
    std::queue<std::string> _logQueue;
    std::atomic<bool> _isRunning;
    std::mutex _mutex;
public:
    std::string _coutNowTimeInfo;
    std::string _objectMessage;
    SystemTimer _systemTimer;

    std::ostream& ioOutput = std::cout;

	StandardOutputLogger(){
        _isRunning=true;
        _threadInstance=new std::thread(&StandardOutputLogger::listen,this);
        log("标准输出日志系统启动");
    }

    ~StandardOutputLogger()
    {
        log("标准输出日志系统关闭");

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

    virtual void setLogLevel(LogLevel level) override
    {
        _logLevel = level;
    }
    
    virtual void log(std::string message)
    {
        if( _logLevel > LogLevel::Log)return;
        push("[   log   ]"+_objectMessage+":" +message);
    }
    virtual void info(std::string message)
    {
        if( _logLevel > LogLevel::Info)return;
        push("[  info   ]"+_objectMessage+":"+message);
    }
    virtual void warning(std::string message)
    {
        if( _logLevel > LogLevel::Warning)return;
        push("[ warning ]"+_objectMessage+":"+message);
    }

    virtual void error(std::string message )
    {
        if( _logLevel > LogLevel::Error)return;
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
        std::lock_guard lock(_mutex);
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
                //TODO: 线程休眠
                std::this_thread::sleep_for(std::chrono::microseconds (100));
            }
        }
    }
};