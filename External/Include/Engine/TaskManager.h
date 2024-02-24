#pragma once

enum TASKTYPE
{
	CHANGE_ROOM, // param 1 : DIRECTION
	CHANGE_CHAPTER, // param 1 : CHAPTER_LEVEL
	CREATE_OBJECT, // param 1 : object, param 2 : layer
	DELETE_OBJECT, // param 1 : object
	CHANGE_ROOMSTATE, // param 1 : room, param 2 : state
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

	bool				m_bCreateObject;
	bool				m_bDeleteObject;

public:
	void Update();

	void AddTask(const Task& _task) { m_TaskQueue.push(_task); }

	bool ObjectEvent() { return m_bCreateObject || m_bDeleteObject; }
};

