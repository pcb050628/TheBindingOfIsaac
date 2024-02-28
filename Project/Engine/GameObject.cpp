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

GameObject::GameObject(const GameObject& _origin)
	: Entity(_origin)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_RoomNumber(-1)
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		AddComponent(_origin.m_Components[i]->Clone());
	}

	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		AddComponent(_origin.m_Scripts[i]->Clone());
	}

	size_t childCount = _origin.m_ChildObjs.size();
	for (size_t i = 0; i < childCount; i++)
	{
		GameObject* childObj = _origin.m_ChildObjs[i]->Clone();
		AttachChild(childObj);
		childObj->m_iLayerIdx = _origin.m_ChildObjs[i]->m_iLayerIdx;
	}
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

	for (size_t i = 0; i < m_Scripts.size(); i++)
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

void GameObject::Enter()
{
	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		m_Scripts[i]->Enter();
	}
}

void GameObject::Update()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if(m_Components[i] != nullptr)
			m_Components[i]->Update();
	}

	for (size_t i = 0; i < m_Scripts.size(); i++)
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
	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		m_Scripts[i]->Exit();
	}
}

void GameObject::BeginOverlap(Collider2D* _other)
{
	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		m_Scripts[i]->BeginOverlap(_other);
	}
}

void GameObject::Overlap(Collider2D* _other)
{
	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		m_Scripts[i]->Overlap(_other);
	}
}

void GameObject::EndOverlap(Collider2D* _other)
{
	for (size_t i = 0; i < m_Scripts.size(); i++)
	{
		m_Scripts[i]->EndOverlap(_other);
	}
}

void GameObject::AddComponent(Component* _comp)
{
	assert(_comp);

	COMPONENT_TYPE type = _comp->GetType();

	_comp->m_Owner = this;

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		((Script*)_comp)->Init();
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
//
//int GameObject::Save()
//{
//	wstring filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + GetName();
//	filePath += L".gobj";
//
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, filePath.c_str(), L"wb");
//
//	if (nullptr == pFile)
//		return E_FAIL;
//
//	//컴포넌트
//	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
//	{
//		bool isExist = false;
//		if (nullptr != m_Components[i]) isExist = true;
//		fwrite(&isExist, sizeof(bool), 1, pFile);
//	}
//
//	//스크립트
//	size_t size = m_Scripts.size();
//	fwrite(&size, sizeof(size_t), 1, pFile);
//	for (auto pair : m_Scripts)
//	{
//		size_t len = pair.first.size();
//		fwrite(&len, sizeof(size_t), 1, pFile);
//		fwrite(pair.first.c_str(), sizeof(wchar_t), len, pFile);
//	}
//
//	return S_OK;
//
//	//std::wofstream fileStream(filePath);
//	//
//	//if (fileStream.is_open())
//	//{
//	//	//RenderComonent 또는 Animator 처럼 원래의 에셋들을 모두 가져오는게 좋은 경우가 있는데 이 경우는 스크립트로 커버하기
//	//	fileStream << L"[COMPONENT]" << std::endl;
//	//	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
//	//	{
//	//		fileStream << i << L"|";
//	//		if (nullptr == m_Components[i])
//	//			fileStream << 0;
//	//		else
//	//			fileStream << 1;
//	//
//	//		fileStream << std::endl;
//	//	}
//	//
//	//	fileStream << L"[SCRIPT]" << std::endl;
//	//	for (auto pair : m_Scripts)
//	//	{
//	//		fileStream << pair.first << std::endl;
//	//	}
//	//
//	//	fileStream << L"END";
//	//
//	//	fileStream.close();
//	//
//	//	return S_OK;
//	//}
//	//else
//	//	return E_FAIL;
//	//
//	//return E_FAIL;
//}
//
//int GameObject::Load(const std::wstring& _strFileName)
//{
//	wstring filePath = GetContentPath() + GetResourceFolderPath(RESOURCE_TYPE::GAMEOBJECT) + _strFileName;
//
//	wchar_t szName[20] = {};
//	_wsplitpath_s(filePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);
//
//	SetName(szName);
//
//	FILE* pFile = nullptr; 
//	_wfopen_s(&pFile, filePath.c_str(), L"rb"); 
//
//	if (nullptr == pFile) 
//		return E_FAIL; 
//
//	//컴포넌트
//	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
//	{
//		bool isExist = false;
//		fread(&isExist, sizeof(bool), 1, pFile);
//		if(isExist) AddComponent(GetComponentByComponentType((COMPONENT_TYPE)i));
//	}
//
//	//스크립트
//	size_t size = 0;
//	fread(&size, sizeof(size_t), 1, pFile);
//	for (int i = 0; i < size; i++)
//	{
//		size_t len = 0;
//		fread(&len, sizeof(size_t), 1, pFile);
//		wstring name;
//		name.resize(len);
//		fread(name.data(), sizeof(wchar_t), len, pFile);
//
//		//Script* scrpt = ScriptManager::GetScript(name);
//		//if (scrpt != nullptr)
//		//	AddComponent(scrpt);
//		//else
//		//{
//		//	MessageBoxW(nullptr, L"Script Not Exist", L"GameObject Load Error", MB_OK);
//		//}
//	}
//
//	return S_OK;
//
//	//std::wifstream fileStream(filePath);
//	//bool component = true;
//	//if (fileStream.is_open())
//	//{
//	//	std::wstring line;
//	//
//	//	while (true)
//	//	{
//	//		std::getline(fileStream, line);
//	//
//	//		if (line == L"END")
//	//			break;
//	//
//	//		if (line == L"[COMPONENT]")
//	//		{
//	//			component = true;
//	//		}
//	//		else if (line == L"[SCRIPT]")
//	//		{
//	//			component = false;
//	//		}
//	//		else
//	//		{
//	//			//RenderComonent 또는 Animator 처럼 원래의 에셋들을 모두 가져오는게 좋은 경우가 있는데 이 경우는 스크립트로 커버하기
//	//			if (component)
//	//			{
//	//				std::wstring comp = line.substr(0, 1);
//	//				std::wstring active = line.substr(2, 1);
//	//
//	//				int iComp = atoi(std::string(comp.begin(), comp.end()).c_str());
//	//				int iActive = atoi(std::string(active.begin(), active.end()).c_str());
//	//
//	//				if (1 == iActive)
//	//				{
//	//					AddComponent(GetComponentByComponentType((COMPONENT_TYPE)iComp));
//	//				}
//	//			}
//	//			else
//	//			{
//	//				Script* scrpt = ScriptFactory::GetInst()->Find(line);
//	//				if (scrpt != nullptr)
//	//					AddComponent(scrpt);
//	//				else
//	//				{
//	//					MessageBoxW(nullptr, L"Script Not Exist", L"GameObject Load Error", MB_OK);
//	//				}
//	//			}
//	//
//	//		}
//	//	}
//	//
//	//	fileStream.close();
//	//	return S_OK;
//	//}
//	//else
//	//	return E_FAIL;
//}
