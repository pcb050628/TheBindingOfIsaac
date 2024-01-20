#include "pch.h"

#include "Entity.h"
#include "TaskManager.h"
#include "GameObject.h"
#include "Layer.h"
#include "Resource.h"

bool IsValid(Entity* _entity)
{
	if (_entity != nullptr && !_entity->GetIsDead())
		return true;
	else
		return false;
}

void ChangeChapter(CHAPTER_LEVEL _level)
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

std::wstring GetResourceFolderPath(RESOURCE_TYPE _type)
{
	std::wstring out = L"Resource\\";

	switch (_type)
	{
	case RESOURCE_TYPE::TEXTURE:
		out += L"Image\\";
		break;
	case RESOURCE_TYPE::ANIM:
		out += L"Anim\\";
		break;
	case RESOURCE_TYPE::MATERIAL:
		out += L"Material\\";
		break;
	case RESOURCE_TYPE::GRAPHICS_SHADER:
		out += L"Shader\\Graphics\\";
		break;
		//case RESOURCE_TYPE::SOUND:
		//	break;
		//case RESOURCE_TYPE::COMPUTER_SHADER:
		//	break;
		//case RESOURCE_TYPE::ROOM:
		//	break;
		//case RESOURCE_TYPE::CHAPTER:
		//	break;
	}

	return out;
}

std::wstring ToWstring(const std::string& _str)
{ 
	return std::wstring(_str.begin(), _str.end()); 
}

std::string ToString(const std::wstring& _str)
{
	return std::string(_str.begin(), _str.end());
}

void Vec3::ToDegree()
{
	x = x * (180 / PI);
	y = y * (180 / PI);
	z = z * (180 / PI);
}

void Vec3::ToRadian()
{
	x = x * (PI / 180);
	y = y * (PI / 180);
	z = z * (PI / 180);
}