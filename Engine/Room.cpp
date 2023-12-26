#include "pch.h"
#include "Room.h"

#include "GameObject.h"

Room::Room() : Super(AssetType::ROOM)
	, m_Layers()
	, m_Type(RoomType::Common)
	, Left(nullptr)
	, Right(nullptr)
	, Top(nullptr)
	, Bottom(nullptr)
{
}

Room::~Room()
{
}

void Room::Create(const std::wstring& _name)
{
}

bool Room::Load(std::wstring _path)
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
