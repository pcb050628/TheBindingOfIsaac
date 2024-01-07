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

private:
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();

	void LoadAllResource(enum class CHAPTERLEVEL _level);

public:
	template <typename T>
	T* Find(std::wstring _strKey)
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
	T* Load(std::wstring _relativePath)
	{
		Resource* tmp = new T();
		tmp->Load(_relativePath);
		if (tmp != nullptr)
		{
			if (IsExist(tmp->GetResourceName(), tmp->GetResourceType()))
			{
				delete tmp;

				// 이미 같은 이름의 리소스가 존재할땐 이미 존재하던 리소스를 반환함
				wchar_t szName[20] = {};
				_wsplitpath_s(_relativePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

				return Find<T>(szName);
			}

			m_Resources[(UINT)tmp->GetResourceType()].insert(std::make_pair(tmp->GetResourceName(), tmp));
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

	RESOURCE_TYPE value = RESOURCE_TYPE::END;

	if (&type == &typeid(Mesh))
		return RESOURCE_TYPE::MESH;
	else if (&type == &typeid(GraphicsShader))
		return RESOURCE_TYPE::GRAPHICS_SHADER;
	else if (&type == &typeid(Material))
		return RESOURCE_TYPE::MATERIAL;
	else if (&type == &typeid(Texture))
		return RESOURCE_TYPE::TEXTURE;
	else if (&type == &typeid(Anim))
		return RESOURCE_TYPE::ANIM;

	return value;
}