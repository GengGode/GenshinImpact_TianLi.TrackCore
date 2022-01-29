//
// Created by GengG on 2022/1/28.
//

#include "HookHandle.h"
#include "Hooker_TengXunYunYouXi.h"
#include "Hooker_YuanShen.h"

HookHandle::HookHandle(const Logger& logService) {
    log = logService;
    _hookers.push_back(std::make_unique<Hooker_YuanShen>());
    _hookers.push_back(std::make_unique<Hooker_TengXunYunYouXi>());
    log->Log("原神句柄模块加载");
}

HookHandle::~HookHandle() {
    log->Log("原神句柄模块卸载");
}

void HookHandle::getValue(HWND &handle) {
    handle=giHandle;
}

bool HookHandle::getGenshinImpactHandle() {
    for(auto &hooker : _hookers) {
        if(_giHandle==NULL||!IsWindow(_giHandle))
        {
            _giHandle=hooker->getGengshinImpactHandle();
        }
        else
        {
            giHandle=_giHandle;
            return true;
        }
    }
    return false;
}
