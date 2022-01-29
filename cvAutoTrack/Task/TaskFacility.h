//
// Created by GengG on 2022/1/28.
//

#ifndef CVAUTOTRACKDLL_TASKFACILITY_H
#define CVAUTOTRACKDLL_TASKFACILITY_H
#include <any>
#include <memory>
class  TaskFacility{

public:
    //virtual ~TaskFacility()=default;
    virtual void beginTask()=0;
    virtual void run()=0;
    virtual void endTask()=0;
    virtual bool isWorkEnd()=0;
    virtual void setStartWork()=0;
    virtual void setStopWork()=0;

    //virtual void setOutputValue(T)=0;
    virtual void setInputValue(std::any value)=0;
    virtual void setOutputValue(std::any value)=0;

    //virtual T getOutputValue()=0;
    virtual std::any getInputValue()=0;
    virtual std::any getOutputValue()=0;
};
//typedef TaskFacility Task;

//template<typename T>
//typedef std::shared_ptr<TaskFacility<T>> Task;
//using Task<T> = std::unique_ptr<TaskFacility<T>>;

using Task = std::unique_ptr<TaskFacility>;
#endif //CVAUTOTRACKDLL_TASKFACILITY_H
