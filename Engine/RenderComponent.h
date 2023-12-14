#pragma once
#include "Component.h"

class RenderComponent :
    public Component
{
private:
    class Mesh*             m_Mesh;
    class GraphicsShader*   m_Shader;

public:
    virtual void Update() {}
    virtual void LateUpate() {}
    virtual void Render() = 0;
    virtual void UpdateData() {}

public:
    void SetMesh(Mesh* _mesh) { m_Mesh = _mesh; }
    void SetShader(GraphicsShader* _shader) { m_Shader = _shader; }

    Mesh* GetMesh() { return m_Mesh; }
    GraphicsShader* GetShader() { return m_Shader; }

public:
    RenderComponent(COMPONENT_TYPE _type);
    virtual ~RenderComponent();
};

