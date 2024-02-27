#pragma once
#include "Resource.h"

class Material :
    public Resource
{
private:
    tMtrlData               m_ConstData;

    class Texture*          m_Textures[(UINT)TEX_PARAM::END];
    class GraphicsShader*   m_Shader;

public:
    virtual bool Load(const std::wstring& _strFilePath, bool _isFullPath) override;
    virtual bool Save() override;

    void UpdateData();

public:
    void SetShader(GraphicsShader* _shader) { m_Shader = _shader; }
    void SetTexture(Texture* _tex, TEX_PARAM _param) { m_Textures[(UINT)_param] = _tex; }

    GraphicsShader* GetShader() { return m_Shader; }

    template<typename T>
    void SetScalarParam(const T& _data, SCALAR_PARAM _param)
    {
        const T* pValue = &_data;

        switch (_param)
        {
        case SCALAR_PARAM::INT_0:
        case SCALAR_PARAM::INT_1:
        case SCALAR_PARAM::INT_2:
        case SCALAR_PARAM::INT_3:
            m_ConstData.iArr[(UINT)_param] = *((int*)pValue);
            break;

        case SCALAR_PARAM::FLOAT_0:
        case SCALAR_PARAM::FLOAT_1:
        case SCALAR_PARAM::FLOAT_2:
        case SCALAR_PARAM::FLOAT_3:
            m_ConstData.fArr[(UINT)_param - (UINT)SCALAR_PARAM::FLOAT_0] = *((float*)pValue);
            break;

        case SCALAR_PARAM::VEC2_0:
        case SCALAR_PARAM::VEC2_1:
        case SCALAR_PARAM::VEC2_2:
        case SCALAR_PARAM::VEC2_3:
            m_ConstData.v2Arr[(UINT)_param - (UINT)SCALAR_PARAM::VEC2_0] = *((Vec2*)pValue);
            break;

        case SCALAR_PARAM::VEC4_0:
        case SCALAR_PARAM::VEC4_1:
        case SCALAR_PARAM::VEC4_2:
        case SCALAR_PARAM::VEC4_3:
            m_ConstData.v4Arr[(UINT)_param - (UINT)SCALAR_PARAM::VEC4_0] = *((Vec4*)pValue);
            break;

        case SCALAR_PARAM::MAT_0:
        case SCALAR_PARAM::MAT_1:
        case SCALAR_PARAM::MAT_2:
        case SCALAR_PARAM::MAT_3:
            m_ConstData.matArr[(UINT)_param - (UINT)SCALAR_PARAM::MAT_0] = *((Matrix*)pValue);
            break;
        }
    }

    virtual Material* Clone() override { return new Material(*this); }

public:
    Material();
    ~Material();
};

