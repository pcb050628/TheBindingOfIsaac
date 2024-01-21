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

    // render 관련
    UINT                        m_LayerCheck;

    std::vector<GameObject*>    m_Opaque;
    std::vector<GameObject*>    m_Masked;
    std::vector<GameObject*>    m_Transparent;
    std::vector<GameObject*>    m_PostProcess;


public:
    virtual void Update() override;
    virtual void LateUpdate() override;

    void SortObject();
    virtual void Render() override;

private:
    void Render(std::vector<GameObject*>& _objs);
    void Render_PostProcess();

public:
    PROJ_TYPE GetProjType() { return m_ProjType; }
    void SetProjType(PROJ_TYPE _type);

    float GetFOV() { return m_FOV; }
    float GetWidth() { return m_Width; }
    float GetScale() { return m_Scale; }
    float GetFar() { return m_Far; }

    void SetFOV(float _fov) { m_FOV = _fov; }
    void SetWidth(float _width) { m_Width = _width; }
    void SetScale(float _scale) { m_Scale = _scale; }
    void SetFar(float _far) { m_Far = _far; }

    void LayerCheckAll(bool _check);
    void LayerCheck(int _layerIdx, bool _check);
    void LayerCheck(LAYER_TYPE _layerType, bool _check);
    bool GetLayerCheck(int _layerType);
    bool GetLayerCheck(LAYER_TYPE _layerType);

    // Camera Register
    void SetCameraPriority(int _Priority);

public:
    Camera();
    ~Camera();
};

