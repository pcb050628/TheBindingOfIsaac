#include "pch.h"
#include "TaskManager.h"
#include "ChapterManager.h"

#include "GameObject.h"

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

		// 모두 수정 필요함
		switch (task.Type)
		{
		case TASKTYPE::CHANGE_ROOM:
			ChapterManager::GetInst()->GetCurChapter()->ChangeRoom((DIRECTION)task.Param_1);
			break;

		case TASKTYPE::CHANGE_CHAPTER:
			ChapterManager::GetInst()->ChangeChapter((CHAPTERLEVEL)task.Param_1);
			break;

		case TASKTYPE::CREATE_OBJECT:
			ChapterManager::GetInst()->GetCurChapter()->AddActor((GameObject*)task.Param_1, (LAYER_TYPE)task.Param_2);
			break;

		case TASKTYPE::DELETE_OBJECT:
			delete (GameObject*)task.Param_1;
			break;
		}

		m_TaskQueue.pop();
	}
}
