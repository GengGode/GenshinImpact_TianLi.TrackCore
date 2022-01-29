//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_TASK_SCREENSHOT_H
#define CVAUTOTRACKDLL_TASK_SCREENSHOT_H
#include "TaskInstance.h"
#include "../GenshinFunction/Screenshot/Screenshot.h"

class Task_Screenshot:public TaskInstance
{
    //构造截图类的独占智能指针
    std::unique_ptr<Screenshot> screenshot;
    std::mutex lm_slot;
public:
    Task_Screenshot(const Logger& logService);

    void beginTask() ;
    void endTask() ;
    void work();
    void slot(HWND handle);
    HWND giHandle;
};




#endif //CVAUTOTRACKDLL_TASK_SCREENSHOT_H
