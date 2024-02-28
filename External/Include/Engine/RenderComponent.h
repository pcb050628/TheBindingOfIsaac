#pragma once
#include "Component.h"

class RenderComponent :
    public Component
{
private:
    class Mesh*         m_Mesh;

    class Material*     m_CurMaterial;
    class Material*     m_SharedMaterial;
    class Material*     m_DynamicMaterial;

public:
    virtual void Update() {}
    virtual void LateUpdate() final {}
    virtual void Render() = 0;
    virtual void UpdateData() {}

public:
    void SetMesh(Mesh* _mesh) { m_Mesh = _mesh; }
    void SetMaterial(Material* _material);

    Mesh* GetMesh() { return m_Mesh; }
    Material* GetMaterial() { return m_CurMaterial; }
    Material* GetDynamicMaterial();

    virtual void SaveToFile(FILE* _file) override;
    virtual void LoadFromFile(FILE* _file) override;

    virtual RenderComponent* Clone() = 0;
public:
    RenderComponent(COMPONENT_TYPE _type);
    RenderComponent(const RenderComponent& _origin);
    virtual ~RenderComponent();
};

