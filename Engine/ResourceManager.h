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
	void Init();

	template <typename T>
	T* Find(ResourceType _type, std::wstring _name) // m_Reources 에서 찾기
	{
		if (m_Resources.find(_name) != m_Resources.end())
			return static_cast<T*>(m_Resources.find(_name)->second);
	}

	template <typename T>
	T* Load(std::wstring _name) // 폴더에서 찾기
	{
		std::wstring fullpath;
		switch (T::GetType())
		{
		case ResourceType::Texture:
			fullpath = L"..\\Resource\\Image\\";
			break;
		case ResourceType::Sound:
			fullpath = L"..\\Resource\\Sound\\";
			break;
		case ResourceType::Anim:
			fullpath = L"..\\Resource\\Anim\\";
			break;
		}

		fullpath += _name;

		

		return nullptr;
	}

	template <typename T>
	T* Create(std::wstring _name) // 생성
	{
		std::wstring fullpath;
		switch (T::GetType())
		{
		case ResourceType::Texture:
			fullpath = L"..\\Resource\\Image\\";
			break;
		case ResourceType::Sound:
			fullpath = L"..\\Resource\\Sound\\";
			break;
		case ResourceType::Anim:
			fullpath = L"..\\Resource\\Anim\\";
			break;
		}

		fullpath += _name;
	}
};

