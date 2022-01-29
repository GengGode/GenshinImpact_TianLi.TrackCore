//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_TASK_HOOKHANDLE_H
#define CVAUTOTRACKDLL_TASK_HOOKHANDLE_H
#include "TaskInstance.h"
#include "GenshinFunction/Flow/FlowFacility.h"
#include "../GenshinFunction/HookHandle/HookHandle.h"

class Task_HookHandle : public TaskInstance {
    //FlowFacility flow;
    // 执行后等待时间 14 毫秒，60Hz
    int _wait_time = 14;

    //构造原神句柄类的独占智能指针
    std::unique_ptr<HookHandle> hookHandle;


public:
    Task_HookHandle(const Logger& logService);
    void beginTask() ;
    void endTask() ;
    void work();

    //void setOutputValue(HWND value);
    //getOutputValue();
};

#endif //CVAUTOTRACKDLL_TASK_HOOKHANDLE_H
