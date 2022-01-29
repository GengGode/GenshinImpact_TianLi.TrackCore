//
// Created by GengG on 2022/1/27.
//

#include "AutoTrackManagement.h"
#include "Logger/ManagementLogger.h"

#include "Task/Task_HookHandle.h"
#include "Task/Task_Screenshot.h"


void AutoTrackManagement::createTasks() {
    tasks.push_back(std::make_unique<Task_HookHandle>(log));
    tasks.push_back(std::make_unique<Task_Screenshot>(log));
    //tasks.push_back(std::make_unique<TaskInstance>());
    //tasks.push_back(std::make_unique<TaskInstance>());

    //task[0].slot(task[1].signal(),HWND);

    log->Log("�����߳�ȫ����ʼ�����");
}

void AutoTrackManagement::beginTasks() {
    for (auto &task : tasks) {
        task->beginTask();
    }
    log->Log("��ʼ�����б�");
}

void AutoTrackManagement::endTasks() {
    for (auto &task : tasks) {
        task->endTask();
    }
    log->Log("�����߳�ȫ������");
}

AutoTrackManagement::AutoTrackManagement() {
    log=std::make_shared<ManagementLogger>();
    datas = std::make_shared<Datas>();

    createTasks();

    beginTasks();

    try
    {
        _thread=new std::thread(&AutoTrackManagement::service,this);
        log->Log("�������̴߳����ɹ�");
    }
    catch (std::exception &e)
    {
        log->Error("�̴߳���ʧ�� " + std::string(e.what()));
    }
    _isRunService=true;
}

AutoTrackManagement::~AutoTrackManagement() {
    endTasks();
    _isRunService=false;
    _isStartService=false;
    if(_thread!= nullptr)_thread->join();
    delete _thread;
}

bool AutoTrackManagement::startService() {
    log->Log("��������");
    _isStartService=true;
    return true;
}

bool AutoTrackManagement::stopService() {
    log->Log("ֹͣ����");
    _isStartService=false;
    return true;
}

void AutoTrackManagement::service() {
    log->Log("����������߳�");
    // ����ѭ���Ƿ����
    while(_isRunService)
    {
        // �Ƿ�ִ������
        if(_isStartService)
        {
            //log->Log("service start");
            //std::this_thread::sleep_for(std::chrono::microseconds (100000));
            if(tasks[0]->isWorkEnd())
            {
                tasks[0]->setStopWork();
                std::any asd=tasks[0]->getOutputValue();


                try
                {
                    HWND asdasd=std::any_cast<HWND>(asd);
                    log->Info(std::to_string((long long int)asdasd));
                }
                catch (const std::bad_any_cast& e)
                {
                    log->Error("��ȡ������ʧ��"+ std::string(e.what()));
                }
                catch (std::exception& e)
                {
                    log->Error("��ȡ������ʧ��"+ std::string(e.what()));
                }


                std::this_thread::sleep_for(std::chrono::seconds (1));

            }
        }
        else
        {
            //log->Log("service stop");
            std::this_thread::sleep_for(std::chrono::microseconds (1));
        }
    }
    log->Log("�����������߳�");
}



bool AutoTrackManagement::GetDirection(double &a) {
    a = datas->_direction.a;
    return datas->_direction.result;
}

bool AutoTrackManagement::GetRotation(double &a2) {
    a2 = datas->_rotation.a2;
    return datas->_rotation.result;
}

bool AutoTrackManagement::GetStarJson(char *string) {
    return false;
}

bool AutoTrackManagement::GetLastError() {
    return false;
}





