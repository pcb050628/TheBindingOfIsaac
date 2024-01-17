#include "pch.h"
#include "GameObject.h"

#include "Script.h"

#include "RenderComponent.h"

#include "ChapterManager.h"
#include "Layer.h"

GameObject::GameObject()
	: m_Components{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
{
}

GameObject::~GameObject()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if (m_Components[i] != nullptr)
		{
			delete m_Components[i];
			m_Components[i] = nullptr;
		}
	}
}

void GameObject::Update()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if(m_Components[i] != nullptr)
			m_Components[i]->Update();
	}

	for (int i = 0; i < m_Scripts.size(); i++)
	{
		m_Scripts[i]->Update();
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

	ChapterManager::GetInst()->RegisterObj(this, (LAYER_TYPE)m_iLayerIdx);

	for (GameObject* child : m_ChildObjs)
	{
		child->LateUpdate();
	}
}

void GameObject::Render()
{
	if(m_RenderComponent != nullptr)
		m_RenderComponent->Render();
}

void GameObject::AddComponent(Component* _comp)
{
	assert(_comp);

	COMPONENT_TYPE type = _comp->GetType();

	assert(!m_Components[(UINT)type]);

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		m_Scripts.push_back((Script*)_comp);
	}
	else
	{
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

void GameObject::DisconnectWithLayer()
{
	if (m_iLayerIdx == -1)
		return;
}
