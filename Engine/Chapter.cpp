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
	m_CurRoom->Clear();

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

void Chapter::DetachGameObject(GameObject* _obj)
{
	m_CurRoom->DetachGameObject(_obj);
}
