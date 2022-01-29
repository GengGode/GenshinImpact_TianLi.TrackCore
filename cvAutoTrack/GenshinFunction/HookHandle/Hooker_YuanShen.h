//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_HOOKER_YUANSHEN_H
#define CVAUTOTRACKDLL_HOOKER_YUANSHEN_H
#include "HookerFacility.h"
//extern "C"
class Hooker_YuanShen : public HookerFacility
{

public:
    HWND getGengshinImpactHandle()
    {
        HWND _giHandle=NULL;
        LPCWSTR giWindowName = { L"原神" };
        /* 对原神窗口的操作 */

        if (_giHandle == NULL)
        {
            giWindowName = L"原神";
            _giHandle = FindWindowW(L"UnityWndClass", giWindowName);
        }
        if (_giHandle == NULL)
        {
            giWindowName = L"Genshin Impact";
            _giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */
        }
        if (_giHandle == NULL)
        {
            giWindowName = L"원신";
            _giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：원신 */
        }
        if (_giHandle == NULL)
        {
            giWindowName = L"\u539F\u795E";
            _giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：原神 */
        }
        if (_giHandle == NULL)
        {
            giWindowName = L"\uC6D0\uC2E0";
            _giHandle = FindWindowW(L"UnityWndClass", giWindowName); /* 匹配名称：원신 */
        }
        return _giHandle;
    }
private:

};
#endif //CVAUTOTRACKDLL_HOOKER_YUANSHEN_H
