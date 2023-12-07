#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
	: m_Components()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (int i = 0; i < ComponentType::End; i++)
	{
		m_Components[i]->Update();
	}
}

void GameObject::LateUpdate()
{
	for (int i = 0; i < ComponentType::End; i++)
	{
		m_Components[i]->LateUpdate();
	}
}

void GameObject::Render()
{
	for (int i = 0; i < ComponentType::End; i++)
	{
		m_Components[i]->Render();
	}
}
