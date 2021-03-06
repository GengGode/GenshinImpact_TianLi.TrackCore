//
// Created by GengG on 2022/1/27.
//

#include "AutoTrackManagement.h"
#include "Logger/ManagementLogger.h"

#include "Task/Task_HookHandle.h"
#include "Task/Task_Screenshot.h"


void AutoTrackManagement::createTasks() {
    tasks.push_back(std::make_unique<Task_HookHandle>(log));
    tasks.push_back(std::make_unique<Task_Screenshot>(log));
    //tasks.push_back(std::make_unique<TaskInstance>());
    //tasks.push_back(std::make_unique<TaskInstance>());

    //task[0].slot(task[1].signal(),HWND);
    
    log->log("任务线程全部初始化完成");
}

void AutoTrackManagement::beginTasks() {
    for (auto &task : tasks) {
        task->beginTask();
    }
    log->log("开始任务列表");
}

void AutoTrackManagement::endTasks() {
    for (auto &task : tasks) {
        task->endTask();
    }
    log->log("任务线程全部结束");
}

AutoTrackManagement::AutoTrackManagement() {
    log=std::make_shared<ManagementLogger>();
    datas = std::make_shared<Datas>();

    createTasks();

    beginTasks();

    try
    {
        _thread=new std::thread(&AutoTrackManagement::service,this);
        log->log("管理主线程创建成功");
    }
    catch (std::exception &e)
    {
        log->error("线程创建失败 " + std::string(e.what()));
    }
    _isRunService=true;
}

AutoTrackManagement::~AutoTrackManagement() {
    endTasks();
    _isRunService=false;
    _isStartService=false;
    if(_thread!= nullptr)_thread->join();
    delete _thread;
}

bool AutoTrackManagement::startService() {
    log->log("开启服务");
    _isStartService=true;
    return true;
}

bool AutoTrackManagement::stopService() {
    log->log("停止服务");
    _isStartService=false;
    return true;
}

void AutoTrackManagement::service() {
    log->log("进入服务主线程");
    // 任务循环是否继续
    while(_isRunService)
    {
        // 是否执行任务
        if(_isStartService)
        {
            //log->Log("service start");
            //std::this_thread::sleep_for(std::chrono::microseconds (100000));
            if(tasks[0]->isWorkEnd())
            {
                tasks[0]->setStopWork();
                std::any asd=tasks[0]->getOutputValue();





                //TODO: 线程休眠
                std::this_thread::sleep_for(std::chrono::seconds (1));

            }
        }
        else
        {
            //TODO: 线程休眠
            std::this_thread::sleep_for(std::chrono::seconds (1));
        }
    }
    log->log("结束服务主线程");
}



bool AutoTrackManagement::GetDirection(double &a) {
    if(datas->_direction.result)
    {
        a = datas->_direction.a;
    }
    return datas->_direction.result;
}

bool AutoTrackManagement::GetRotation(double &a2) {
    if(datas->_rotation.result)
    {
        a2 = datas->_rotation.a2;
    }
    return datas->_rotation.result;
}

bool AutoTrackManagement::GetStarJson(char *string) {
    return false;
}

bool AutoTrackManagement::GetLastError() {
    return false;
}





