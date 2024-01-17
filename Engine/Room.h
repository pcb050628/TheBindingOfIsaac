#pragma once
#include "Resource.h"
#include "Layer.h"

enum DIRECTION
{
    LeftDir,
    RightDir,
    TopDir,
    BottomDir,
};

enum class RoomType
{
    Common,
    Treasure,
    Shop,
    Boss,
};

class Room :
    public Resource
{
private:
    RoomType m_Type;
    Layer* m_Layers[(int)LAYER_TYPE::END];

    // gameobject.prefab 파일로 저장하기

    Room* Left;
    Room* Right;
    Room* Top;
    Room* Bottom;

    bool m_bEditMode;

public:
    virtual bool Load(const std::wstring& _strFilePath) override;
    virtual bool Save() override;

    virtual void Clear();

    virtual void Enter();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Exit();

    virtual Layer* GetLayer(int _type) { return m_Layers[_type]; }
    virtual Layer* GetLayer(LAYER_TYPE _type) { return GetLayer((int)_type); }

    // 15(14) x 9(8) tile / include walls

    virtual Room* GetRoomByDir(DIRECTION _dir)
    {
        switch (_dir)
        {
        case LeftDir:
            return Left;
            break;
        case RightDir:
            return Right;
            break;
        case TopDir:
            return Top;
            break;
        case BottomDir:
            return Bottom;
            break;
        }

        return nullptr;
    }

    //x row / y col
    Vec2 GetPosByTile(Vec2 _tile);
    Vec2 GetTileByPos(Vec2 _pos);

    void AddObject(GameObject* _obj, LAYER_TYPE _layr, bool _bMove)
    {
        m_Layers[(UINT)_layr]->AddObject(_obj, _bMove);
    }

    void AddObjectByTile(GameObject* _obj, LAYER_TYPE _layr, Vec2 _tilePos, bool _bMove);

    void DetachGameObject(GameObject* _obj);

    GameObject* FindObject(const std::wstring& _strName);

    void SetEditMode(bool _bValue) { m_bEditMode = _bValue; }

public:
    Room();
    virtual ~Room() override;

};

