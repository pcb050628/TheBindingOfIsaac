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

class GameObject;
class Layer :
    public Entity
{
private:
    vector<GameObject*> m_Actors;

public:
    void Update();
    void LateUpdate();
    void Render();

    // ���߿� �����Ҷ� ����Ұ�
    vector<GameObject*> GetActorAboveTileY(int _y);
    vector<class Collider*> GetAllCollider();

    void AddActor(GameObject* _actr)
    {
        m_Actors.push_back(_actr);
    }

public:
    Layer();
    ~Layer() override;
};

