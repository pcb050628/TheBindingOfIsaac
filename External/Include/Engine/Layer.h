#pragma once
#include "Entity.h"

enum class LAYER_TYPE
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
    vector<GameObject*> m_Parents;
    vector<GameObject*> m_Gobjs;

    int                 m_iLayerIdx;

public:
    void Update();
    void LateUpdate();
    void Render();

    void Clear();

    void AddObject(GameObject* _obj, bool _bMove = false);
    void DetachGameObject(GameObject* _obj);

    void RegisterObject(GameObject* _obj);

    const vector<GameObject*>& GetGameObject() { return m_Gobjs; }

public:
    Layer();
    ~Layer() override;

    friend class Room;
};

