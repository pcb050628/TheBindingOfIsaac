#pragma once
#include "Entity.h"

#include "Component.h"

#include "components.h"

#define GET_COMPONENT(Type, TYPE) Type* Get##Type() { return (Type*)m_Components[(UINT)COMPONENT_TYPE::##TYPE]; }  

class Script;

class GameObject :
    public Entity
{
private:
    Component*                  m_Components[(UINT)COMPONENT_TYPE::END];
    std::vector<Script*>        m_Scripts;
    RenderComponent*            m_RenderComponent;

    std::vector<GameObject*>    m_ChildObjs;

    GameObject*                 m_Parent;

    int                         m_iLayerIdx;

public:
    virtual void Update(); 
    virtual void LateUpdate();
    virtual void Render();

    void AddComponent(Component* _comp);
    
    Component* GetComponent(COMPONENT_TYPE _type)
    {
        return m_Components[(UINT)_type];
    }

    GET_COMPONENT(Transform,    TRANSFORM);
    GET_COMPONENT(MeshRenderer, MESHRENDERER);
    GET_COMPONENT(Camera,       CAMERA);
    GET_COMPONENT(Light2D,      LIGHT2D);
    GET_COMPONENT(Collider2D,   COLLIDER2D);
    GET_COMPONENT(Animator2D,   ANIMATOR2D);

    template <typename T>
    T* GetScript()
    {
        for (int i = 0; i < m_Scripts.size(); i++)
        {
            if (nullptr != dynamic_cast<T*>(m_Scripts[i]))
                return (T*)m_Scripts[i];
        }
    }

    RenderComponent* GetRenderCom() { return m_RenderComponent; }

    GameObject* GetParent() { return m_Parent; }
    void AttachChild(GameObject* _objChild);

    void DisconnectWithParent();
    void DisconnectWithLayer();

private:
    void Destroy() { SetDead(); }

public:
    GameObject();
    virtual ~GameObject() override;

    friend class Layer;
    friend class Room;
    friend class TaskManager;
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
    else if (&info == &typeid(class Animator2D))
        type = COMPONENT_TYPE::ANIMATOR2D;

    return type;
}