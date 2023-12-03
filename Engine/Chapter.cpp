#include "pch.h"
#include "Chapter.h"
#include "TaskManager.h"

Chapter::Chapter()
	: m_Rooms()
	, m_CurRoom(nullptr)
{
}

Chapter::~Chapter()
{
}

void Chapter::Update()
{
	m_CurRoom->Update();
}

void Chapter::LateUpdate()
{
	m_CurRoom->LateUpdate();
}

void Chapter::Render()
{
	m_CurRoom->Render();
}

void ChangeRoom(DIRECTION _dir)
{
	Task task = {};
	task.Type = TASKTYPE::CHANGE_ROOM;
	task.Param_1 = (UINT_PTR)_dir;

	TaskManager::GetInst()->AddTask(task);
}
