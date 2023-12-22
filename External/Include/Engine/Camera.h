#pragma once
#include "Component.h"

enum class PROJ_TYPE
{
    ORTHOGRAPHICS,
    PERSPECTIVE,
};

class Camera :
    public Component
{
private:
    PROJ_TYPE m_ProjType;

    // perspective
    float   m_FOV;          

    // orthographic
    float   m_Width;        
    float   m_Scale;        

    // both
    float   m_AspectRatio;  
    float   m_Far;          

    // 변환 행렬
    Matrix  m_matView;
    Matrix  m_matProj;

public:
    virtual void Update() override;
    virtual void LateUpdate() override;

public:
    float GetFOV() { return m_FOV; }
    float GetScale() { return m_Scale; }

    void SetFOV(float _fov) { m_FOV = _fov; }
    void SetScale(float _scale) { m_Scale = _scale; }

public:
    Camera();
    ~Camera();
};

