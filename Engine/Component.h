#pragma once
#include "Entity.h"

enum ComponentType
{
    STATEMACHINE,
    COLLIDER,
    RENDERER,
    ANIMATOR,

    End,
};

class Component :
    public Entity
{
    typedef Entity Super;

private:
    class GameObject* m_Owner;
    const ComponentType m_Type;

public:
    virtual void Update() = 0;
    virtual void LateUpdate() = 0;
    virtual void Render();

    GameObject* GetOwner() { return m_Owner; }

    virtual const ComponentType GetType() { return m_Type; }

public:
    Component(ComponentType _type);
    virtual ~Component();

    friend GameObject;
};

