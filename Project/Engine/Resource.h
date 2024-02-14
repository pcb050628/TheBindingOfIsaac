#pragma once
#include "Entity.h"
#include "define.h"

enum class RESOURCE_TYPE
{
	TEXTURE,
	ANIM,
	SOUND,
	MESH,
	GRAPHICS_SHADER,
	COMPUTE_SHADER,
	MATERIAL,
	ROOM,
	CHAPTER,
	END,
	GAMEOBJECT,
};

class Resource : 
	public Entity
{
protected:
	std::wstring m_ResourcePath;
	std::wstring m_ResourceName;

	const RESOURCE_TYPE	m_Type;

public:
	virtual bool Load(const std::wstring& _FileName, bool _isFullPath) { return false; }
	virtual bool Save() { return false; }

	virtual void SetResourcePath(std::wstring _path) { m_ResourcePath = _path; }
	virtual std::wstring GetResourcePath() { return m_ResourcePath; }

	virtual void SetResourceName(std::wstring _name) { m_ResourceName = _name; }
	virtual std::wstring GetResourceName() { return m_ResourceName; }

	virtual RESOURCE_TYPE GetResourceType() { return m_Type; }

	virtual Resource* Clone() { return nullptr; }

public:
	Resource(RESOURCE_TYPE _type);
	virtual ~Resource() override;

	friend class ResouceManager;
};

