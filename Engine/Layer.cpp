#include "pch.h"
#include "Layer.h"
#include "Actor.h"
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
	for (Actor* actor : m_Actors)
	{
		actor->Update();
	}
}

void Layer::LateUpdate()
{
	for (Actor* actor : m_Actors)
	{
		actor->LateUpdate();
	}
}

vector<Actor*> Layer::GetActorAboveTileY(int _y)
{
	vector<Actor*> value = {};

	for (Actor* actor : m_Actors)
	{
		if (static_cast<int>(actor->GetPosition().y) == _y)
			value.push_back(actor);
	}

	return value;
}

vector<Collider*> Layer::GetAllCollider()
{
	vector<Collider*> vec = {};

	for (Actor* act : m_Actors)
	{
		for (int i = 0; i < act->GetComponent(COLLIDER)->size(); i++)
		{
			vec.push_back(static_cast<Collider*>(act->GetComponent(COLLIDER)->at(i)));
		}
	}

	return vec;
}
