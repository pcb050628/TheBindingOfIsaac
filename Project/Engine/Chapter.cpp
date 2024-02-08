#include "pch.h"
#include "Chapter.h"
#include "TaskManager.h"

#include "Device.h"

#include "GameObject.h"
#include "components.h"

Chapter::Chapter()
	: m_Rooms()
	, m_CurRoom(nullptr)
	, m_bIsTransitioning(false)
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
	_room->m_Info.RoomNumber = (UINT)m_Rooms.size();
	m_Rooms.push_back(_room);
}

void Chapter::Update()
{
	m_CurRoom->Clear();
	if (!m_bIsTransitioning)
		m_CurRoom->Update();
	else
	{
		ChangeRoomTransition();
	}
}

void Chapter::LateUpdate()
{
	if(!m_bIsTransitioning)
		m_CurRoom->LateUpdate();
}

void Chapter::DetachGameObject(GameObject* _obj)
{
	m_CurRoom->DetachGameObject(_obj);
}

void Chapter::GenerateRooms(CHAPTER_LEVEL _level)
{

}

void Chapter::ChangeRoomStart(DIRECTION _dir)
{
	m_CurRoom->Exit();
	m_bIsTransitioning = true;
	m_ChangeDir = _dir;

	Room* changeRoom = m_CurRoom->GetRoomByDir(m_ChangeDir);
	assert(changeRoom);
	GameObject* cam = changeRoom->GetMainCam();

	Vec3 sub(0.f);

	switch (m_ChangeDir)
	{
	case LeftDir:
		sub = Vec3(RENDER_RESOLUTION.x / 2, 0, 0);
		break;
	case RightDir:
		sub = Vec3(-(RENDER_RESOLUTION.x / 2), 0, 0);
		break;
	case TopDir:
		sub = Vec3(0, -(RENDER_RESOLUTION.y / 2), 0);
		break;
	case BottomDir:
		sub = Vec3(0, RENDER_RESOLUTION.y / 2, 0);
		break;
	}

	cam->GetTransform()->AddRelativePos(sub);
}

void Chapter::ChangeRoomTransition()
{
	if (!m_bIsTransitioning)
		return;

	Room* changeRoom = m_CurRoom->GetRoomByDir(m_ChangeDir); 
	assert(changeRoom); 

	GameObject* curCam = m_CurRoom->GetMainCam();
	GameObject* changeCam = changeRoom->GetMainCam(); 

	Vec3 dir(0.f);

	float speed = 100.f;

	switch (m_ChangeDir)
	{
	case LeftDir:
		dir = Vec3(-1, 0, 0);
		break;
	case RightDir:
		dir = Vec3(1, 0, 0);
		break;
	case TopDir:
		dir = Vec3(0, 1, 0);
		break;
	case BottomDir:
		dir = Vec3(0, -1, 0);
		break;
	}

	Vec3 vel = dir * speed;
	curCam->GetTransform()->AddRelativePos(vel);
	changeCam->GetTransform()->AddRelativePos(vel);

	if (changeCam->GetTransform()->GetRelativePos().x <= 0.f)
	{
		ChangeRoomEnd();
	}
}

void Chapter::ChangeRoomEnd()
{
	m_bIsTransitioning = false;
	m_CurRoom = m_CurRoom->GetRoomByDir(m_ChangeDir);
	m_CurRoom->GetMainCam()->GetTransform()->SetRelativePos(DEFAULT_CAMERA_POS);
	m_CurRoom->Enter();

	TaskManager::GetInst()->SetDoSomething(true);
}