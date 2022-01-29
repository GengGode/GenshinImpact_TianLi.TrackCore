#pragma once
#include "WorkFacility.h"
#include "../Task/TaskInstance.h"

/// <summary>
/// 工作节点，内置线程
/// 有一个输入，一个输出
/// 
/// </summary>
class WorkNode :public WorkFacility
{
	TaskInstance _task;
	std::function<void(void)> _func;
public:
	WorkNode() {
		
		_task.beginTask();

	}
	~WorkNode() {
		_task.endTask();

	}
	void setTask(std::function<void(void)> func)
	{
		_task._task = func;
	}
	void setFlowSignal(std::function<void(void)> func)
	{
		_func = func;
	}

private:
	void work()
	{
		startWork();
		sendOutputSignal();
	}
	bool tryGetInput()
	{

	}
	bool checkInputIsCanWork()
	{

	}
	bool startWork() 
	{

	}

private:
	void sendOutputSignal()
	{
		_func();
	}
};

