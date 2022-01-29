//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_HOOKER_TENGXUNYUNYOUXI_H
#define CVAUTOTRACKDLL_HOOKER_TENGXUNYUNYOUXI_H
#include "HookerFacility.h"
#include <string>
#include <functional>

HWND callbcakHandle;
int callbackCount;
extern "C" BOOL CALLBACK EnumGameChildProc(
        HWND handle,      // handle to child window
        LPARAM lpa    // application-defined value
);
extern "C" BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam);

class Hooker_TengXunYunYouXi : public HookerFacility
{
public:
    HWND getGengshinImpactHandle()
    {
        char classNameLis[256];
        char nameLis[256];
        HWND _giHandle=NULL;

        _giHandle = GetForegroundWindow();

        GetClassNameA(_giHandle, classNameLis, 256);
        giClass = classNameLis;

        GetWindowTextA(_giHandle, nameLis, 256);
        giName = nameLis;

        if (giName == "腾讯云游戏")
        {
            callbackCount = 0;
            EnumWindows(EnumWindowsProc, NULL);
            HWND parantHandle = callbcakHandle;//= GetNextWindow(callbcakHandle, GW_HWNDNEXT);
            EnumChildWindows(parantHandle, EnumGameChildProc, NULL);
            HWND outHandle = callbcakHandle;

            if (IsWindow(outHandle))
            {
                _giHandle = outHandle;
            }
            GetClassNameA(_giHandle, classNameLis, 256);
            giClass = classNameLis;

            GetWindowTextA(_giHandle, nameLis, 256);
            giName = nameLis;
        }
        else
        {
            _giHandle = NULL;
        }

        return _giHandle;
    }
private:
    std::string giName="";
    std::string giClass="";
};



extern "C" BOOL CALLBACK EnumGameChildProc(
        HWND handle,      // handle to child window
        LPARAM lpa    // application-defined value
)
{
    callbackCount++;
    if (callbackCount == 2)
    {
        callbcakHandle = handle;
        return false;
    }
    return true;
}

extern "C" BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
    //GetWindow
    char nameLis[256];
    GetWindowTextA(hwnd, nameLis, 256);

    if (std::string(nameLis) == "腾讯云游戏")
    {
        RECT r;
        GetClientRect(hwnd, &r);
        if (r.right - r.left > 1000 && r.bottom - r.top > 600)
        {
            callbcakHandle = hwnd;
        }
    }
    return true;
}

#endif //CVAUTOTRACKDLL_HOOKER_TENGXUNYUNYOUXI_H
