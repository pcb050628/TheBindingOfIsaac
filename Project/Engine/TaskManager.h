#pragma once

enum TASKTYPE
{
	CHANGE_ROOM,
	CHANGE_CHAPTER,
	CREATE_OBJECT,
	DELETE_OBJECT,
};

struct Task
{
	TASKTYPE Type;
	UINT_PTR Param_1;
	UINT_PTR Param_2;

	Task()
		: Type()
		, Param_1()
		, Param_2()
	{}
};

class TaskManager
{
	SINGLETON(TaskManager)
private:
	std::queue<Task>	m_TaskQueue;

	bool				m_bDoSomething;

public:
	void Update();

	void AddTask(const Task& _task) { m_TaskQueue.push(_task); }

	bool IsDoSomething() { return m_bDoSomething; }
	void SetDoSomething(bool _value) { m_bDoSomething = _value; }
};

