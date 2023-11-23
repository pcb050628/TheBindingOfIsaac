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
    void Render();

    // ���߿� �����Ҷ� ����Ұ�
    vector<Actor*> GetActorAboveTileY(int _y);
    vector<class Collider*> GetAllCollider();

public:
    Layer();
    ~Layer() override;
};

