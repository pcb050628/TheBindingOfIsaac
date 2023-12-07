#pragma once
#include "Entity.h"

#include "Component.h"

class GameObject :
    public Entity
{
    typedef Entity Super;

private:
    Component* m_Components[(UINT)ComponentType::End];

public:
    virtual void Update(); 
    virtual void LateUpdate();
    virtual void Render();

    template <typename T>
    T* AddComponent(const wstring& _name = L"")
    {
        Component* comp = new T();
        comp->SetName(_name);
        comp->m_Owner = this;
        m_Components[(UINT)comp->GetType()] = comp;
        return dynamic_cast<T*>(comp);
    }

    Component* GetComponent(ComponentType _type)
    {
        return m_Components[(UINT)_type];
    }

public:
    GameObject();
    virtual ~GameObject() override;

};

