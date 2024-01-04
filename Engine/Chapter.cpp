#include "pch.h"
#include "Chapter.h"
#include "TaskManager.h"

Chapter::Chapter()
	: m_Rooms()
	, m_CurRoom(nullptr)
{
	m_CurRoom = new Room();
	m_Rooms.push_back(m_CurRoom);
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

void Chapter::GenerateRooms(CHAPTERLEVEL _level)
{
	// ������� room ���µ��� é�ʹܰ迡 �°� �����ϰ� ��ġ���ʰ� �ҷ�����
}
