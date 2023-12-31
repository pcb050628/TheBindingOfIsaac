#include "pch.h"
#include "Room.h"

#include "GameObject.h"

Room::Room() : Resource(RESOURCE_TYPE::ROOM)
	, m_Layers()
	, m_Type(RoomType::Common)
	, Left(nullptr)
	, Right(nullptr)
	, Top(nullptr)
	, Bottom(nullptr)
{
	for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
	{
		m_Layers[i].m_iLayerIdx = i;
	}
}

Room::~Room()
{
}

bool Room::Load(const std::wstring& _strFilePath)
{
	return false;
}

bool Room::Save()
{
	return false;
}

void Room::Clear()
{
	for (Layer& layer : m_Layers)
	{
		layer.Clear();
	}
}

void Room::Enter()
{
}

void Room::Update()
{
	for (Layer& layer : m_Layers)
	{
		layer.Update();
	}
}

void Room::LateUpdate()
{
	for (Layer& layer : m_Layers)
	{
		layer.LateUpdate();
	}
}

void Room::Render()
{
	for (Layer& layer : m_Layers)
	{
		layer.Render();
	}
}

void Room::Exit()
{
}


void Room::DetachGameObject(GameObject* _obj)
{
	m_Layers[_obj->m_iLayerIdx].DetachGameObject(_obj);
}
