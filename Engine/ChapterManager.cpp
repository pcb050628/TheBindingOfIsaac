#include "pch.h"
#include "ChapterManager.h"
#include "Chapter.h"

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
