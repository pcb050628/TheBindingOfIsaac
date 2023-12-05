#pragma once
#include "Entity.h"

enum class AssetType
{
    TEXTURE,
    ANIM,
    ROOM,
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
    virtual void SetAssetPath(std::wstring _path) { m_Path = _path; }
    virtual std::wstring GetAssetPath() { return m_Path; }

    virtual void SetAssetName(std::wstring _name) { m_ResourceName = _name; }
    virtual std::wstring GetAssetName() { return m_ResourceName; }

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
    CHAPTER1_1,
    CHAPTER1_2,
    CHAPTER1_3,
    CHAPTER1_4,
    CHAPTER2_1,
    CHAPTER2_2,
    CHAPTER2_3,
    CHAPTER2_4,
    CHAPTER3_1,
    CHAPTER3_2,
    CHAPTER3_3,
    CHAPTER3_4,
    CHAPTER4_1,
    CHAPTER4_2,
    CHAPTER4_3,
    CHAPTER4_4,
};