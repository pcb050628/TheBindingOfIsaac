#include "pch.h"
#include "Room.h"

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

void Room::Create(AssetID _id)
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

