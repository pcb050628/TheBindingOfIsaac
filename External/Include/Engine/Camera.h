#pragma once
#include "Component.h"

class Camera :
    public Component
{
private:
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
    Camera();
    ~Camera();
};

