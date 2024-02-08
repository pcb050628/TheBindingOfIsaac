#include "pch.h"
#include "GameObject.h"

#include "Script.h"

#include "RenderComponent.h"

#include "ScriptFactory.h"
#include "ChapterManager.h"
#include "Layer.h"

GameObject::GameObject()
	: m_Components{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_RoomNumber(-1)
{
}

GameObject::GameObject(const std::wstring& _name)
	: m_Components{}
	, m_RenderComponent(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_RoomNumber(-1)
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

	auto pair = m_Scripts.begin();
	for (; pair != m_Scripts.end(); )
	{
		delete pair->second;
		pair = m_Scripts.erase(pair);
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

void GameObject::Enter()
{
	for (auto pair : m_Scripts)
	{
		pair.second->Enter();
	}
}

void GameObject::Update()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if(m_Components[i] != nullptr)
			m_Components[i]->Update();
	}

	for (auto pair : m_Scripts)
	{
		pair.second->Update();
	}

	for (GameObject* child : m_ChildObjs)
	{
		child->Update();
	}
}

void GameObject::LateUpdate()
{
	ChapterManager::GetInst()->RegisterObj(this, (LAYER_TYPE)m_iLayerIdx);

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
}

void GameObject::Exit()
{
	for (auto pair : m_Scripts)
	{
		pair.second->Exit();
	}
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
		m_Scripts.insert(make_pair(_comp->GetName(), (Script*)_comp));
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

void GameObject::DeleteComponent(COMPONENT_TYPE _type)
{
	if (nullptr == m_Components[(UINT)_type])
		return;

	if (m_Components[(UINT)_type] == m_RenderComponent)
		m_RenderComponent = nullptr;
	delete m_Components[(UINT)_type];
	m_Components[(UINT)_type] = nullptr;

}

void GameObject::DeleteScript(const std::wstring& _name)
{
	auto iter = m_Scripts.find(_name);
	if (m_Scripts.end() == iter)
		return;

	delete iter->second;
	m_Scripts.erase(iter);
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
	if (!m_Parent)
		return;

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

void GameObject::GetScriptName(std::vector<std::string>& _out)
{
	if (m_Scripts.empty())
		return;

	for (auto pair : m_Scripts)
	{
		_out.push_back(ToString(pair.first));
	}
}

int GameObject::Save()
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + GetName();
	filePath += L".gobj";
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		//RenderComonent 또는 Animator 처럼 원래의 에셋들을 모두 가져오는게 좋은 경우가 있는데 이 경우는 스크립트로 커버하기
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
		for (auto pair : m_Scripts)
		{
			fileStream << pair.first << std::endl;
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
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + _strFileName;
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(filePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	SetName(szName);

	bool component = true;
	if (fileStream.is_open())
	{
		std::wstring line;

		while (true)
		{
			std::getline(fileStream, line);

			if (line == L"END")
				break;

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
				//RenderComonent 또는 Animator 처럼 원래의 에셋들을 모두 가져오는게 좋은 경우가 있는데 이 경우는 스크립트로 커버하기
				if (component)
				{
					std::wstring comp = line.substr(0, 1);
					std::wstring active = line.substr(2, 1);

					int iComp = atoi(std::string(comp.begin(), comp.end()).c_str());
					int iActive = atoi(std::string(active.begin(), active.end()).c_str());

					if (1 == iActive)
					{
						AddComponent(GetComponentByComponentType((COMPONENT_TYPE)iComp));
					}
				}
				else
				{
					Script* scrpt = ScriptFactory::GetInst()->Find(line);
					if (scrpt != nullptr)
						AddComponent(scrpt);
					else
					{
						MessageBoxW(nullptr, L"Script Not Exist", L"GameObject Load Error", MB_OK);
					}
				}

			}
		}

		fileStream.close();
		return S_OK;
	}
	else
		return E_FAIL;
}
