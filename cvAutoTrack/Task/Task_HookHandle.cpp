//
// Created by GengG on 2022/1/28.
//

#include <functional>
#include "Task_HookHandle.h"
#include "../Logger/TasksLogger.h"

Task_HookHandle::Task_HookHandle(const Logger &logService) : TaskInstance(logService) {
    //给原神句柄类的智能指针制造实例
    hookHandle=std::make_unique<HookHandle>(logService);

}

void Task_HookHandle::beginTask() {
    _task=std::bind(&Task_HookHandle::work,this);
    _isStartService=true;
    log->log("Task_HookHandle::beginTask()");
    TaskInstance::beginTask();
}

void Task_HookHandle::endTask() {
    TaskInstance::endTask();
}

void Task_HookHandle::work() {
    //flow.asyncGetValue();
    if(hookHandle->getGenshinImpactHandle())
    {
        hookHandle->giHandle;
        setOutputValue(hookHandle->giHandle.load());
        // 找到原神或云原神句柄
        log->log("找到原神或云原神句柄");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    else
    {
        //等待1000毫秒
        hookHandle->giHandle=NULL;
        log->log("没有找到原神窗口，等待 500ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds (_wait_time));
}
