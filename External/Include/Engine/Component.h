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

    virtual void SaveToFile(FILE* _file) = 0;
    virtual void LoadFromFile(FILE* _file) = 0;

    virtual Component* Clone() = 0;
public:
    Component(COMPONENT_TYPE _type);
    Component(const Component& _origin);
    virtual ~Component();

    friend GameObject;
};

