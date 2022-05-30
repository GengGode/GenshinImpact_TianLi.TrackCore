//
// Created by GengG on 2022/1/28.
//

#include <functional>
#include "Task_HookHandle.h"
#include "../Logger/TasksLogger.h"

Task_HookHandle::Task_HookHandle(const Logger &logService) : TaskInstance(logService) {
    //��ԭ�����������ָ������ʵ��
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
        // �ҵ�ԭ�����ԭ����
        log->log("�ҵ�ԭ�����ԭ����");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    else
    {
        //�ȴ�1000����
        hookHandle->giHandle=NULL;
        log->log("û���ҵ�ԭ�񴰿ڣ��ȴ� 500ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds (_wait_time));
}
