#include "pch.h"

#include "Entity.h"
#include "TaskManager.h"
#include "GameObject.h"
#include "Layer.h"


bool IsValid(Entity* _entity)
{
	if (_entity != nullptr && !_entity->GetIsDead())
		return true;
	else
		return false;
}

void ChangeChapter(CHAPTERLEVEL _level)
{
	Task task;
	task.Type = TASKTYPE::CHANGE_CHAPTER;
	task.Param_1 = (UINT_PTR)_level;

	//TaskManager::GetInst()->AddTask(task);
}

void AddGameObject(GameObject* _actr, LAYER_TYPE _layer)
{
	Task task;
	task.Type = TASKTYPE::CREATE_OBJECT;
	task.Param_1 = (UINT_PTR)_actr;
	task.Param_2 = (UINT_PTR)_layer;

	TaskManager::GetInst()->AddTask(task);
}

void ChangeRoom(DIRECTION _dir)
{
	Task task = {};
	task.Type = TASKTYPE::CHANGE_ROOM;
	task.Param_1 = (UINT_PTR)_dir;

	TaskManager::GetInst()->AddTask(task);
}

void Destroy(GameObject* _obj)
{
	Task task = {};
	task.Type = TASKTYPE::DELETE_OBJECT;
	task.Param_1 = (UINT_PTR)_obj;

	TaskManager::GetInst()->AddTask(task);
}

std::wstring GetContentPath()
{
	wchar_t path[500];
	GetModuleFileNameW(nullptr, path, 500);
	std::wstring directoryPath(path);

	for (int i = 0; i < 2; i++)
	{
		size_t point = directoryPath.find_last_of(L"\\");

		directoryPath = directoryPath.substr(0, point);
	}

	return directoryPath + L"\\Content\\";
}