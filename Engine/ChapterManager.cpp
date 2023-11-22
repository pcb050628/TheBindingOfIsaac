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

void ChapterManager::LateUpdate()
{
	m_CurChapter->LateUpdate();
}
