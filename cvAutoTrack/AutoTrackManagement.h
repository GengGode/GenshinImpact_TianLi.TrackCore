//
// Created by GengG on 2022/1/27.
//

#ifndef CVAUTOTRACKDLL_AUTOTRACKMANAGEMENT_H
#define CVAUTOTRACKDLL_AUTOTRACKMANAGEMENT_H


#include "Logger/LoggingFacility.h"
#include "Task/TaskFacility.h"
#include "Data/DataFacility.h"
#include "Data/Data_GenshinImpact.h"

#include "GenshinFunction/HookHandle/HookHandle.h"
#include <vector>
#include <thread>

class AutoTrackManagement {
    Logger log;
    std::vector<Task> tasks;

    //std::vector<Data<std::any>> datas;
    //Datas datas;
    std::shared_ptr<Datas> datas;

    std::thread* _thread= nullptr;
    std::atomic<bool> _isRunService;
    std::atomic<bool> _isStartService;
private:
    void createTasks();
    void beginTasks();
    void endTasks();

public:
    AutoTrackManagement();
    ~AutoTrackManagement();

    bool startService();
    bool stopService();

    void service();

    /// 获取角色朝向
    /// \param a 输出角色朝向
    /// \return 是否成功获取
    bool GetDirection(double &a);

    bool GetRotation(double &a2);

    bool GetStarJson(char *string);
    bool GetLastError();
};


#endif //CVAUTOTRACKDLL_AUTOTRACKMANAGEMENT_H
