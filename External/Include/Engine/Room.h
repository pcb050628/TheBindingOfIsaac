#pragma once
#include "Asset.h"
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
    public Asset
{
    typedef Asset Super;
private:
    RoomType m_Type;
    Layer m_Layers[(int)LayerType::END];

    Room* Left;
    Room* Right;
    Room* Top;
    Room* Bottom;

public:
    virtual bool Load(std::wstring _path) override;
    virtual bool Save() override;

    virtual void Enter();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();
    virtual void Exit();

    virtual Layer& GetLayer(LayerType _type) { return m_Layers[(int)_type]; }

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

    void AddActor(GameObject* _actr, LayerType _layr)
    {
        m_Layers[(UINT)_layr].AddActor(_actr);
    }

public:
    Room();
    virtual ~Room() override;

};

