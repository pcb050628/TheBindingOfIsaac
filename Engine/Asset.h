#pragma once
#include "Entity.h"

enum class AssetType
{
    TEXTURE,
    ANIM,
    END
};

enum class AssetID;

class Asset :
    public Entity
{
private:
    AssetID m_ID;
    std::wstring m_Path;
    std::wstring m_ResourceName;

    static AssetType m_Type;

private:
    virtual bool Load(std::wstring _path) = 0;
    virtual bool Save() = 0;

    void SetID(AssetID _id) { m_ID = _id; }

public:
    virtual void SetResourcePath(std::wstring _path) { m_Path = _path; }
    virtual std::wstring GetResourcePath() { return m_Path; }

    virtual void SetResourceName(std::wstring _name) { m_ResourceName = _name; }
    virtual std::wstring GetResourceName() { return m_ResourceName; }

    AssetID GetAssetID() { return m_ID; }

    static AssetType GetType() { return m_Type; }

public:
    Asset(AssetType _type);
    virtual ~Asset(); 

    friend class AssetManager;
};


enum class AssetID
{
    TEXTURE_CHAPTER1_BACKGROUND,
    TEXTURE_CHAPTER2_BACKGROUND,
    TEXTURE_CHAPTER3_BACKGROUND,
    TEXTURE_CHAPTER4_BACKGROUND,
    ANIM_PLAYER_WALK_HORIZONTAL,
    ANIM_PLAYER_WALK_VERTICAL,
};