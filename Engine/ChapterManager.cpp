#include "pch.h"
#include "ChapterManager.h"
#include "TaskManager.h"

ChapterManager::ChapterManager()
	: m_CurChapter(nullptr)
	, m_Chapters{}
{

}

ChapterManager::~ChapterManager()
{
	for (Chapter* chptr : m_Chapters)
	{
		delete chptr;
	}
}

void ChapterManager::Init()
{
	for (Chapter* chptr : m_Chapters)
	{
		chptr = new Chapter();
	}
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

void ChapterManager::DetachGameObject(GameObject* _obj)
{
	m_CurChapter->DetachGameObject(_obj);
}

void ChapterManager::RegisterObj(GameObject* _obj, LAYER_TYPE _layerType)
{
	m_CurChapter->GetCurRoom()->GetLayer(_layerType).RegisterObject(_obj);
}

