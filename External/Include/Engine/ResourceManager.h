#pragma once
#include "pch.h"
#include "define.h"
#include "Resource.h"

#include "resources.h"

class ResourceManager
{
	SINGLETON(ResourceManager);
private:
	std::map<std::wstring, Resource*> m_Resources[(UINT)RESOURCE_TYPE::END];
	
public:
	void Init();

public:
	template <typename T>
	T* Find(std::wstring _strKey) // m_Reources 에서 찾기
	{
		RESOURCE_TYPE type = GetResourceType<T>();

		if (m_Resources[(UINT)type].find(_strKey) != m_Resources[(UINT)type].end())
			return dynamic_cast<T*>(m_Resources[(UINT)type].find(_strKey)->second);
		return nullptr;
	}

	bool IsExist(const std::wstring& _strKey, RESOURCE_TYPE _type)
	{
		if (m_Resources[(UINT)_type].find(_strKey) != m_Resources[(UINT)_type].end())
			return true;
		else
			return false;
	}

	template <typename T>
	T* LoadByPath(std::wstring _strName, std::wstring _path) // 폴더에서 찾기
	{
		T* f = Find<T>(_strName); 
		if (f != nullptr) 
			return f; 

		Resource* tmp = new T();
		tmp->Load(_path);
		if (tmp != nullptr)
		{
			tmp->SetResourceName(_strName);
			m_Resources[(UINT)tmp->GetResourceType()].insert(std::make_pair(_strName, tmp));
			return dynamic_cast<T*>(tmp);
		}
		else
			delete tmp;

		return nullptr;
	}

	bool AddResource(std::wstring _strKey, Resource* _resource)
	{
		if (IsExist(_strKey, _resource->GetResourceType()))
			return false;

		m_Resources[(UINT)_resource->GetResourceType()].insert(std::make_pair(_strKey, _resource));
		return true;
	}
};

template <typename T>
RESOURCE_TYPE GetResourceType()
{
	const type_info& type = typeid(T);

	if (&type == &typeid(class Mesh))
		return RESOURCE_TYPE::MESH;
	else if (&type == &typeid(class GraphicsShader))
		return RESOURCE_TYPE::GRAPHICS_SHADER;
}