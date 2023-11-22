#pragma once
#include "Entity.h"
#include "math.h"
#include "Component.h"

class Actor :
    public Entity
{
    typedef Entity Super;

private:
    Vec2 m_Position;
    Vec2 m_Scale;
    vector<Component*>* m_Components[ComponentType::End];

public:
    virtual void Update(); 
    virtual void LateUpdate();

    void SetPosition(Vec2 _pos) { m_Position = _pos; }
    void SetScale(Vec2 _scale) { m_Scale = _scale; }
    Vec2 GetPosition() { return m_Position; }
    Vec2 GetScale() { return m_Scale; }
    
    template <typename T>
    T* AddComponent(const wstring& _name = L"")
    {
        Component* comp = new T();
        comp->SetName(_name);
        comp->m_Owner = this;
        m_Components[comp->GetType()]->push_back(comp);
        return static_cast<T*>(comp);
    }

    vector<Component*>* GetComponent(ComponentType _type)
    {
        return m_Components[_type];
    }

    Component* GetComponent(ComponentType _type, const wstring& _name)
    {
        for (int i = 0; i < m_Components[_type]->size(); i++)
        {
            if (m_Components[_type]->at(i)->GetName() == _name)
                return m_Components[_type]->at(i);
        }

        return nullptr;
    }

    void Destroy() { Super::mb_IsDead = true; }

public:
    Actor();
    virtual ~Actor() override;

};

