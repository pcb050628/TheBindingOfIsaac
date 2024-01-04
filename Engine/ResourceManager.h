#pragma once
#include "pch.h"
#include "define.h"
#include "Resource.h"
#include "directxtk/SpriteBatch.h"

class ResourceManager
{
	SINGLETON(ResourceManager);
private:
	std::map<std::wstring, Resource*> m_Resources;

public:
	template <typename T>
	T* Find(std::wstring _name) // m_Reources 에서 찾기
	{
		if (m_Resources.find(_name) != m_Resources.end())
			return dynamic_cast<T*>(m_Resources.find(_name)->second);
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
			m_Resources.insert(std::make_pair(_name, tmp));
			return dynamic_cast<T*>(tmp);
		}
		else
			delete tmp;

		return nullptr;
	}
};

