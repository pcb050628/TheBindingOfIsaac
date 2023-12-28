#pragma once
#include "Resource.h"

class Material :
    public Resource
{
private:
    tMaterial               m_ConstData;

    class Texture*          m_Textures[TEX_PARAM::END];
    class GraphicsShader*   m_Shader;

public:
    virtual bool Load(const std::wstring& _strFilePath);
    virtual bool Save();

    void UpdateData();

public:
    void SetShader(GraphicsShader* _shader) { m_Shader = _shader; }
    void SetTexture(Texture* _tex, TEX_PARAM _param) { m_Textures[_param] = _tex; }


public:
    Material();
    ~Material();
};

