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

    template<typename T>
    void SetScalarParam(const T& _data, SCALAR_PARAM _param)
    {
        switch (_param)
        {
        case INT_0:
        case INT_1:
        case INT_2:
        case INT_3:
            m_ConstData.iArr[_param] = (int)_data;
            break;

        case FLOAT_0:
        case FLOAT_1:
        case FLOAT_2:
        case FLOAT_3:
            m_ConstData.fArr[_param - FLOAT_0] = (float)_data;
            break;

        case VEC2_0:
        case VEC2_1:
        case VEC2_2:
        case VEC2_3:
            m_ConstData.v2Arr[_param - VEC2_0] = (Vec2)_data;
            break;

        case VEC4_0:
        case VEC4_1:
        case VEC4_2:
        case VEC4_3:
            m_ConstData.v4Arr[_param - VEC4_0] = (Vec4)_data;
            break;

        case MAT_0:
        case MAT_1:
        case MAT_2:
        case MAT_3:
            m_ConstData.matArr[_param - MAT_0] = (Matrix)_data;
            break;
        }
    }

public:
    Material();
    ~Material();
};

