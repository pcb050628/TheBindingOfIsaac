#include "pch.h"
#include "TaskManager.h"
#include "ChapterManager.h"
#include "GarbageCollection.h"

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

		switch (task.Type)
		{
		case TASKTYPE::CHANGE_ROOM:
			ChapterManager::GetInst()->GetCurChapter()->ChangeRoomStart((DIRECTION)task.Param_1);
			break;

		case TASKTYPE::CHANGE_CHAPTER:
			ChapterManager::GetInst()->ChangeChapter((CHAPTER_LEVEL)task.Param_1);
			break;

		case TASKTYPE::CREATE_OBJECT:
			ChapterManager::GetInst()->GetCurChapter()->AddObject((GameObject*)task.Param_1, (LAYER_TYPE)task.Param_2, false);
			break;

		case TASKTYPE::DELETE_OBJECT:
		{
			std::queue<GameObject*> queueObj;
			queueObj.push(((GameObject*)task.Param_1));
			while (!queueObj.empty())
			{
				GameObject* obj = queueObj.front();

				for (int i = 0; i < obj->m_ChildObjs.size(); i++)
				{
					queueObj.push(obj->m_ChildObjs[i]);
				}

				obj->Destroy();
				queueObj.pop();
			}
		}
			break;
		}

		m_TaskQueue.pop();
	}
}
