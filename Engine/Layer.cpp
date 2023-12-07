#include "pch.h"
#include "Layer.h"
#include "GameObject.h"
#include "Collider.h"

Layer::Layer()
	: m_Actors()
{
}

Layer::~Layer()
{
	auto iter = m_Actors.begin();
	for (; iter != m_Actors.end(); )
	{
		delete (*iter);
		iter = m_Actors.erase(iter);
	}
}

void Layer::Update()
{
	for (GameObject* actor : m_Actors)
	{
		actor->Update();
	}
}

void Layer::LateUpdate()
{
	for (GameObject* actor : m_Actors)
	{
		actor->LateUpdate();
	}
}

void Layer::Render()
{
	for (GameObject* actor : m_Actors)
	{
		actor->Render();
	}

	auto iter = m_Actors.begin();
	for (; iter != m_Actors.end();)
	{
		if ((*iter)->GetIsDead())
			iter = m_Actors.erase(iter);
		else
			iter++;
	}
}

vector<GameObject*> Layer::GetActorAboveTileY(int _y) // 사용할 일 없을듯
{
	vector<GameObject*> value = {};

	for (GameObject* actor : m_Actors)
	{
	}

	return value;
}

vector<Collider*> Layer::GetAllCollider()
{
	vector<Collider*> vec = {};

	for (GameObject* act : m_Actors)
	{

	}

	return vec;
}
