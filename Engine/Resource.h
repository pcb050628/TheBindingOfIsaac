#pragma once
#include "Entity.h"
#include "define.h"

enum class ResourceType
{
	Texture,
	Sound,
	MESH,
	GRAPHICS_SHADER,
	COMPUTER_SHADER,
	End
};

class Resource : 
	public Entity
{
protected:
	std::wstring m_ResourcePath;
	std::wstring m_ResourceName;

	const ResourceType	m_Type;

public:
	virtual bool Load(std::wstring _FilePath) = 0;
	virtual bool Save();

	virtual void SetResourcePath(std::wstring _path) { m_ResourcePath = _path; }
	virtual std::wstring GetResourcePath() { return m_ResourcePath; }

	virtual void SetResourceName(std::wstring _name) { m_ResourceName = _name; }
	virtual std::wstring GetResourceName() { return m_ResourceName; }

public:
	Resource(ResourceType _type);
	virtual ~Resource() override;

	friend class ResouceManager;
};

