#pragma once
#include "Entity.h"
#include "Layer.h"

enum class RoomType
{
    Common,
    Treasure,
    Shop,
    Boss,
};

class Room :
    public Entity
{
private:
    Layer m_Layers[(int)LayerType::END];

public:
    virtual void Enter();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();
    virtual void Exit();

    virtual Layer& GetLayer(LayerType _type) { return m_Layers[(int)_type]; }

    // 15 x 9 tile

public:
    Room();
    virtual ~Room() override;
    
};

