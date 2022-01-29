//
// Created by GengG on 2022/1/29.
//

#include "Task_Screenshot.h"

Task_Screenshot::Task_Screenshot(const Logger& logService) :TaskInstance(logService) {
    //给截图类的智能指针制造实例
    screenshot=std::make_unique<Screenshot>(logService);
}
void Task_Screenshot::beginTask() {
    TaskInstance::beginTask();
    _task=std::bind(&Task_Screenshot::work,this);
    _isStartService=true;
}

void Task_Screenshot::endTask() {
    std::lock_guard<std::mutex> lock(lm_slot);
    TaskInstance::endTask();
}

void Task_Screenshot::work() {
    std::lock_guard<std::mutex> lock(lm_slot);
    screenshot->setValue(giHandle);
    screenshot->work();
}

void Task_Screenshot::slot(HWND handle) {
    std::lock_guard<std::mutex> lock(lm_slot);
    giHandle=handle;
}

