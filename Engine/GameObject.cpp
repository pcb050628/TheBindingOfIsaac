#include "pch.h"
#include "GameObject.h"

#include "RenderComponent.h"
#include "Script.h"

GameObject::GameObject()
	: m_Components()
	, m_Scripts()
	, m_RenderComponent(nullptr)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		m_Components[i]->Update();
	}

	for (Script* scrpt : m_Scripts)
	{
		scrpt->Update();
	}
}

void GameObject::LateUpdate()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		m_Components[i]->LateUpdate();
	}
}

void GameObject::Render()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		m_RenderComponent->Render();
	}
}

void GameObject::AddComponent(Component* _comp)
{
	assert(_comp);

	COMPONENT_TYPE type = _comp->GetType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		m_Scripts.push_back((Script*)_comp);
	}
	else
	{
		assert(!m_Components[(UINT)type]);

		m_Components[(UINT)type] = _comp;

		if (nullptr != dynamic_cast<RenderComponent*>(_comp))
		{
			assert(!m_RenderComponent);
			m_RenderComponent = (RenderComponent*)_comp;
		}
	}

	_comp->m_Owner = this;
}
