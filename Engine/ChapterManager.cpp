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
