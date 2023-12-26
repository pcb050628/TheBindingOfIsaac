#pragma once
#include "Entity.h"

#include "Component.h"

#include "components.h"

class RenderComponent;

class GameObject :
    public Entity
{
    typedef Entity Super;

private:
    Component*                  m_Components[(UINT)COMPONENT_TYPE::END];
    RenderComponent*            m_RenderComponent;

    std::vector<GameObject*>    m_ChildObjs;

    GameObject*                 m_Parent;

public:
    virtual void Update(); 
    virtual void LateUpdate();
    virtual void Render();

    void AddComponent(Component* _comp);
    
    template <typename T>
    T* GetComponent()
    {
        return (T*)m_Components[(UINT)GetCompType<T>()];
    }

    GameObject* GetParent() { return m_Parent; }
    void AttachChild(GameObject* _objChild);
    void DisconnectWithParent();

public:
    GameObject();
    virtual ~GameObject() override;
};

template <typename T>
COMPONENT_TYPE GetCompType()
{
    const type_info& info = typeid(T);

    COMPONENT_TYPE type = COMPONENT_TYPE::END;

    if (&info == &typeid(Transform))
        type = COMPONENT_TYPE::TRANSFORM;
    else if (&info == &typeid(MeshRenderer))
        type = COMPONENT_TYPE::MESHRENDERER;
    else if (&info == &typeid(class Camera))
        type = COMPONENT_TYPE::CAMERA;

    return type;
}