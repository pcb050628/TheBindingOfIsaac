#include "pch.h"
#include "ChapterManager.h"
#include "TaskManager.h"

void ChapterManager::Init()
{

}

void ChapterManager::Update()
{
	m_CurChapter->Update();
}

void ChapterManager::Render()
{
	m_CurChapter->Render();
}

void ChapterManager::LateUpdate()
{
	m_CurChapter->LateUpdate();
}

void ChangeChapter(CHAPTERLEVEL _level)
{
	Task task;
	task.Type = TASKTYPE::CHANGE_CHAPTER;
	task.Param_1 = (UINT_PTR)_level;
}

void AddActor(GameObject* _actr, CHAPTERLEVEL _level)
{
	Task task;
	task.Type = TASKTYPE::CREATE_ACTOR;
	task.Param_1 = (UINT_PTR)_actr;
	task.Param_2 = (UINT_PTR)_level;
}
