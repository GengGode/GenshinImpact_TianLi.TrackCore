//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_WORKFACILITY_H
#define CVAUTOTRACKDLL_WORKFACILITY_H
#include <memory>

class WorkFacility
{
public:
    /// 尝试获取输入的参数
    /// \return 成果或者失败
    virtual bool tryGetInput()=0;
    /// 检查输入的值是否可以进行工作
    /// \return 合格或者不合格的输入
    virtual bool checkInputIsCanWork()=0;
    /// 开始工作
    /// \return 成功或者失败
    virtual bool startWork()=0;
    
    virtual void sendOutputSignal() = 0;
};
using work = std::unique_ptr<WorkFacility>;

#icnldue "../../Task/Task.h"
#include "../../Task/Task_HookHandle.h"

class test:public WorkFacility
{
public:
    Task task;
    test()
    {
        task=std::make_unique<Task_HookHandle>();
    }
    bool tryGetInput()
    {
        task.beginTask();
    }
};
#endif //CVAUTOTRACKDLL_WORKFACILITY_H
