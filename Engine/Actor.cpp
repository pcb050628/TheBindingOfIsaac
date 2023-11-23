#include "pch.h"
#include "Actor.h"

Actor::Actor()
	: m_Position()
	, m_Scale()
	, m_Components()
{
}

Actor::~Actor()
{
}

void Actor::Update()
{
	for (int i = 0; i < ComponentType::End; i++)
	{
		for (int comp = 0; comp < m_Components[i]->size(); comp++)
		{
			m_Components[i]->at(comp)->Update();
		}
	}
}

void Actor::LateUpdate()
{
	for (int i = 0; i < ComponentType::End; i++)
	{
		for (int comp = 0; comp < m_Components[i]->size(); comp++)
		{
			m_Components[i]->at(comp)->LateUpdate();
		}
	}
}

void Actor::Render()
{
	for (int i = 0; i < ComponentType::End; i++)
	{
		for (int comp = 0; comp < m_Components[i]->size(); comp++)
		{
			m_Components[i]->at(comp)->Render();
		}
	}
}
