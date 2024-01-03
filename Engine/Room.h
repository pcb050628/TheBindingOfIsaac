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
    Layer m_Layers[(int)LAYER_TYPE::END];

    Room* Left;
    Room* Right;
    Room* Top;
    Room* Bottom;

public:
    virtual bool Load(const std::wstring& _strFilePath) override;
    virtual bool Save() override;

    virtual void Clear();

    virtual void Enter();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();
    virtual void Exit();

    virtual Layer* GetLayer(int _type) { return &m_Layers[_type]; }
    virtual Layer* GetLayer(LAYER_TYPE _type) { return GetLayer((int)_type); }

    // 15 x 9 tile

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

    void AddObject(GameObject* _obj, LAYER_TYPE _layr, bool _bMove)
    {
        m_Layers[(UINT)_layr].AddObject(_obj, _bMove);
    }

    void DetachGameObject(GameObject* _obj);

public:
    Room();
    virtual ~Room() override;

};

