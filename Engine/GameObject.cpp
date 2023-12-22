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
		if(m_Components[i] != nullptr)
			m_Components[i]->Update();
	}

	for (Script* scrpt : m_Scripts)
	{
		scrpt->Update();
	}

	for (GameObject* child : m_ChildObjs)
	{
		child->Update();
	}
}

void GameObject::LateUpdate()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if (m_Components[i] != nullptr)
			m_Components[i]->LateUpdate();
	}

	for (GameObject* child : m_ChildObjs)
	{
		child->LateUpdate();
	}
}

void GameObject::Render()
{
	if(m_RenderComponent != nullptr)
		m_RenderComponent->Render();

	for (GameObject* child : m_ChildObjs)
	{
		child->Render();
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

void GameObject::AttachChild(GameObject* _objChild)
{
	if (_objChild->GetParent())
	{
		_objChild->DisconnectWithParent();
	}

	_objChild->m_Parent = this;
	m_ChildObjs.push_back(_objChild);
}

void GameObject::DisconnectWithParent()
{
	std::vector<GameObject*>::iterator iter = m_Parent->m_ChildObjs.begin();
	for (; iter != m_ChildObjs.end(); iter++)
	{
		if (*iter == this)
		{
			m_Parent->m_ChildObjs.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr); // 부모 객체가 존재하는데 그 객체의 자식 벡터에 내가 없음
}
