#include "pch.h"
#include "TaskManager.h"
#include "ChapterManager.h"

#include "Actor.h"

TaskManager::TaskManager()
{

}

TaskManager::~TaskManager()
{
	while (!m_TaskQueue.empty())
	{
		m_TaskQueue.pop();
	}
}

void TaskManager::Update()
{
	while (!m_TaskQueue.empty())
	{
		Task& task = m_TaskQueue.front();

		switch (task.Type)
		{
		case TASKTYPE::CHANGE_ROOM:
			ChapterManager::GetInst()->GetCurChapter()->ChangeRoom((DIRECTION)task.Param_1);
			break;

		case TASKTYPE::CHANGE_CHAPTER:
			ChapterManager::GetInst()->ChangeChapter((CHAPTERLEVEL)task.Param_1);
			break;

		case TASKTYPE::CREATE_ACTOR:
			ChapterManager::GetInst()->GetCurChapter()->AddActor((Actor*)task.Param_1, (LayerType)task.Param_2);
			break;

		case TASKTYPE::DELETE_ACTOR:
			delete (Actor*)task.Param_1;
			break;
		}

		m_TaskQueue.pop();
	}
}
