#include "pch.h"
#include "Chapter.h"
#include "TaskManager.h"


Chapter::Chapter()
	: m_Rooms()
	, m_CurRoom(nullptr)
	, m_bEditMode(false)
{
	m_CurRoom = new Room;
	AddRoom(m_CurRoom);
}

Chapter::~Chapter()
{
	for (int i = 0; i < m_Rooms.size(); i++)
	{
		delete m_Rooms[i];
	}
	m_Rooms.clear();
}

void Chapter::AddRoom(Room* _room)
{
	_room->m_RoomNumber = (UINT)m_Rooms.size();
	m_Rooms.push_back(_room);
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

void Chapter::DetachGameObject(GameObject* _obj)
{
	m_CurRoom->DetachGameObject(_obj);
}

void Chapter::GenerateRooms(CHAPTER_LEVEL _level)
{
	// ������� room ���µ��� é�ʹܰ迡 �°� �����ϰ� ��ġ���ʰ� �ҷ�����
}

void Chapter::SetEditMode(bool _bValue)
{
	m_bEditMode = _bValue;
	m_CurRoom->SetEditMode(_bValue);
}
