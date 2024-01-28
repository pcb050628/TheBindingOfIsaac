#include "pch.h"
#include "GameObject.h"

#include "Script.h"

#include "RenderComponent.h"

#include "ScriptFactory.h"
#include "ChapterManager.h"
#include "Layer.h"

#define ALLOCATE_SCRIPT(Name) new Name;

GameObject::GameObject()
	: m_Components{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
{
}

GameObject::GameObject(const std::wstring& _name)
	: m_Components{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
{
	SetName(_name);
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

	for (int i = 0; i < m_Scripts.size(); i++)
	{
		if (m_Scripts[i] != nullptr)
		{
			delete m_Scripts[i];
			m_Scripts[i] = nullptr;
		}
	}

	for (int i = 0; i < m_ChildObjs.size(); i++)
	{
		if (m_ChildObjs[i] != nullptr)
		{
			delete m_ChildObjs[i];
			m_ChildObjs[i] = nullptr;
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

	_comp->m_Owner = this;

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		((Script*)_comp)->Init();
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

	ChapterManager::GetInst()->GetCurChapter()->DetachGameObject(this);
	m_iLayerIdx = -1;
}

int GameObject::Save()
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + GetName();
	filePath += L".txt";
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		fileStream << L"[COMPONENT]" << std::endl;
		for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
		{
			fileStream << i << L"|";
			if (nullptr == m_Components[i])
				fileStream << 0;
			else
				fileStream << 1;

			fileStream << std::endl;
		}

		fileStream << L"[SCRIPT]" << std::endl;
		for (int i = 0; i < m_Scripts.size(); i++)
		{
			fileStream << typeid(m_Scripts[i]).hash_code() << std::endl;
		}

		fileStream << L"END";

		fileStream.close();

		return S_OK;
	}
	else
		return E_FAIL;

	return E_FAIL;
}

int GameObject::Load(const std::wstring& _strFileName)
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + GetName();
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(filePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	SetName(szName);

	bool component = true;
	if (fileStream.is_open())
	{
		std::wstring line;

		while (line != L"END")
		{
			std::getline(fileStream, line);

			if (line == L"[COMPONENT]")
			{
				component = true;
			}
			else if (line == L"[SCRIPT]")
			{
				component = false;
			}
			else
			{
				if (component)
				{

				}
				else
				{
					ScriptFactory::GetInst()->Find(atoi(std::string(line.begin(), line.end()).c_str()));
				}
			}
		}

		fileStream.close();
		return S_OK;
	}
	else
		return E_FAIL;
}
