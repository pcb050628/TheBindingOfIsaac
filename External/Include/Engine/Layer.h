#pragma once
#include "Entity.h"

class GameObject;
class Layer :
    public Entity
{
private:
    vector<GameObject*> m_Parents;
    vector<GameObject*> m_Gobjs;

    int                 m_iLayerIdx;

public:
    void Enter();
    void Update();
    void LateUpdate();
    void Render();
    void Exit();

    void Clear();
    void DeleteAll();

    void AddObject(GameObject* _obj, bool _bMove = false);
    void DetachGameObject(GameObject* _obj);

    void RegisterObject(GameObject* _obj);

    vector<GameObject*> GetParentObjects() { return m_Parents; }
    const vector<GameObject*>& GetGameObject() { return m_Gobjs; }

    GameObject* GetGameObject(GameObject* _obj);
    GameObject* GetGameObject(const wstring& _name);

    CLONE(Layer)
public:
    Layer();
    Layer(const Layer& _origin);
    ~Layer() override;

    friend class Room;
};

