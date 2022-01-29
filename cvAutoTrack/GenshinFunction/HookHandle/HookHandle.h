//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_HOOKHANDLE_H
#define CVAUTOTRACKDLL_HOOKHANDLE_H
#include "../../Logger/LoggingFacility.h"

#include <windows.h>
#include <atomic>
#include <vector>

#include "HookerFacility.h"

//原神句柄类
class  HookHandle
{
    HWND _giHandle;
    std::vector<Hooker> _hookers;

    Logger log;
public:
    std::atomic<HWND> giHandle;
    // 禁用默认构造函数
    HookHandle() = delete;

    HookHandle(const Logger& logService);
    ~HookHandle();
    bool getGenshinImpactHandle();
    void getValue(HWND& handle);

};
#endif //CVAUTOTRACKDLL_HOOKHANDLE_H

