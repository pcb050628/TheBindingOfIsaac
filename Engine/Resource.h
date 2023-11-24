#pragma once
#include "pch.h"
#include "define.h"

enum class ResourceType
{
	Texture,
	Sound,
	Anim,
	End
};

class Resource
{
private:
	std::wstring m_ResourcePath;
	std::wstring m_ResourceName;

	static ResourceType m_Type;

public:
	static ResourceType GetType() { return m_Type; }

	virtual bool Load(std::wstring _FilePath) = 0;
	virtual Resource* Create();
	virtual bool Save() = 0;

	virtual void SetResourcePath(std::wstring _path) { m_ResourcePath = _path; }
	virtual std::wstring GetResourcePath() { return m_ResourcePath; }

	virtual void SetResourceName(std::wstring _name) { m_ResourceName = _name; }
	virtual std::wstring GetResourceName() { return m_ResourceName; }

public:
	Resource(ResourceType _type);
	virtual ~Resource();

	friend class ResouceManager;
};

