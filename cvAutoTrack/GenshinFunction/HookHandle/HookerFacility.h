//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_HOOKERFACILITY_H
#define CVAUTOTRACKDLL_HOOKERFACILITY_H
#include <windows.h>
#include <memory>

class HookerFacility {
public:
    virtual HWND  getGengshinImpactHandle()=0;
};
using Hooker =std::unique_ptr<HookerFacility>;
#endif //CVAUTOTRACKDLL_HOOKERFACILITY_H
