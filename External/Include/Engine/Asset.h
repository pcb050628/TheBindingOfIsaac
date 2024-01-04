#pragma once
#include "Entity.h"

enum class AssetType
{
    TEXTURE,
    ANIM,
    ROOM,
    END
};

class Asset :
    public Entity
{
protected:
    std::wstring m_Path;
    std::wstring m_AssetName;

    const AssetType m_Type;

private:
    virtual bool Load(std::wstring _path) = 0;
    virtual bool Save() = 0;
    virtual bool Create(std::wstring _resourcePath, std::wstring _resourceName) = 0;

public:
    virtual void SetAssetPath(std::wstring _path) { m_Path = _path; }
    virtual std::wstring GetAssetPath() { return m_Path; }

    virtual void SetAssetName(std::wstring _name) { m_AssetName = _name; }
    virtual std::wstring GetAssetName() { return m_AssetName; }

    virtual AssetType GetAssetType() { return m_Type; }

public:
    Asset(AssetType _type);
    virtual ~Asset(); 

    friend class AssetManager;
};
