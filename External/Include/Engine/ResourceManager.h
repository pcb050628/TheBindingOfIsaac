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
	T* Find(std::wstring _name) // m_Reources 에서 찾기
	{
		RESOURCE_TYPE type = GetResourceType<T>();

		if (m_Resources[(UINT)type].find(_name) != m_Resources[(UINT)type].end())
			return dynamic_cast<T*>(m_Resources[(UINT)type].find(_name)->second);
		return nullptr;
	}

	template <typename T>
	T* LoadByPath(std::wstring _name, std::wstring _path) // 폴더에서 찾기
	{
		T* f = Find<T>(_name); 
		if (f != nullptr) 
			return f; 

		Resource* tmp = new T();
		tmp->Load(_path);
		if (tmp != nullptr)
		{
			tmp->SetResourceName(_name);
			m_Resources[(UINT)tmp->GetResourceType()].insert(std::make_pair(_name, tmp));
			return dynamic_cast<T*>(tmp);
		}
		else
			delete tmp;

		return nullptr;
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