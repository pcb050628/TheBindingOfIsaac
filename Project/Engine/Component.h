#pragma once
#include "Entity.h"

class Component :
    public Entity
{
    typedef Entity Super;

private:
    class GameObject* m_Owner;
    const COMPONENT_TYPE m_Type;

public:
    virtual void Update() {}
    virtual void LateUpdate() = 0;
    virtual void Render() {}
    virtual void UpdateData() {}

    GameObject* GetOwner() { return m_Owner; }

    virtual const COMPONENT_TYPE GetType() { return m_Type; }

public:
    Component(COMPONENT_TYPE _type);
    virtual ~Component();

    friend GameObject;
};

