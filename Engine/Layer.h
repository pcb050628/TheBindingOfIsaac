#pragma once
#include "Entity.h"

enum class LayerType
{
    Background,
    Object,
    Monster,
    Player,
    Pickup,
    Item,
    END,
};

class Actor;
class Layer :
    public Entity
{
private:
    vector<Actor*> m_Actors;

public:
    void Update();
    void LateUpdate();

    // 나중에 렌더할때 사용할것
    vector<Actor*> GetActorAboveTileY(int _y);
    vector<class Collider*> GetAllCollider();

public:
    Layer();
    ~Layer() override;
};

