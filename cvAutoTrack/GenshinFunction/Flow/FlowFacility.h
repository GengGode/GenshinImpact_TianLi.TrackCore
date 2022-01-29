//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_FLOWFACILITY_H
#define CVAUTOTRACKDLL_FLOWFACILITY_H
#include <atomic>
#include <functional>
#include <memory>
#include <any>

class FlowFacility {
public:
    virtual void asyncSetOInputValue(std::any in) =0;
    virtual void asyncGetOutputValue(std::any out) =0;
    //bool checkInput();
};

using Flow = std::unique_ptr<FlowFacility>;


#endif //CVAUTOTRACKDLL_FLOWFACILITY_H
