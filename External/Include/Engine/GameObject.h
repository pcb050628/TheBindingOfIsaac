#pragma once
#include "Entity.h"

#include "Component.h"

class Script;
class RenderComponent;

class GameObject :
    public Entity
{
    typedef Entity Super;

private:
    Component*              m_Components[(UINT)COMPONENT_TYPE::END];
    RenderComponent*        m_RenderComponent;
    std::vector<Script*>    m_Scripts;

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

public:
    GameObject();
    virtual ~GameObject() override;
};

template <typename T>
COMPONENT_TYPE GetCompType()
{
    const type_info& info = typeid(T);

    if (&info == &typeid(class Transform))
        return COMPONENT_TYPE::TRANSFORM;
    else if (&info == &typeid(class MeshRenderer))
        return COMPONENT_TYPE::MESHRENDERER;
    /*else if (&info == &typeid(class Camera))
        return COMPONENT_TYPE::CAMERA;*/
}