#pragma once
#include "Component.h"

class RenderComponent :
    public Component
{
private:
    class Mesh*         m_Mesh;
    class Material*     m_Material;

public:
    virtual void Update() {}
    virtual void LateUpdate() final {}
    virtual void Render() = 0;
    virtual void UpdateData() {}

public:
    void SetMesh(Mesh* _mesh) { m_Mesh = _mesh; }
    void SetMaterial(Material* _material) { m_Material = _material; }

    Mesh* GetMesh() { return m_Mesh; }
    Material* GetMaterial() { return m_Material; }

public:
    RenderComponent(COMPONENT_TYPE _type);
    virtual ~RenderComponent();
};

