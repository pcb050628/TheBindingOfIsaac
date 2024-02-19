#pragma once
#include "Shader.h"

struct ScalarParamDesc
{
    SCALAR_PARAM    type;
    string          desc;
};
struct TexParamDesc
{
    TEX_PARAM   type;
    string      desc;
};

enum class ShaderOrder 
{
    VS,
    HS,
    DS,
    GS,
    PS,
    END,
};

struct ShaderInfo
{
    std::wstring path = L"NULL";
    std::string func = "NULL";
};

class GraphicsShader :
    public Shader
{
private:
    Microsoft::WRL::ComPtr<ID3DBlob>                m_pVSBlob;
    Microsoft::WRL::ComPtr<ID3DBlob>                m_pHSBlob;
    Microsoft::WRL::ComPtr<ID3DBlob>                m_pDSBlob;
    Microsoft::WRL::ComPtr<ID3DBlob>                m_pGSBlob;
    Microsoft::WRL::ComPtr<ID3DBlob>                m_pPSBlob;

    Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_pVertexShader;
    Microsoft::WRL::ComPtr<ID3D11HullShader>        m_pHullShader;
    Microsoft::WRL::ComPtr<ID3D11DomainShader>      m_pDomainShader;
    Microsoft::WRL::ComPtr<ID3D11GeometryShader>    m_pGeometryShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pPixelShader;

    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_pInputLayout;

    D3D11_PRIMITIVE_TOPOLOGY                        m_Topology;

    RS_TYPE                                         m_RSType;
    DSS_TYPE                                        m_DSSType;
    BS_TYPE                                         m_BSType;

    ShaderInfo                                      m_ShaderInfos[(UINT)ShaderOrder::END];

    SHADER_DOMAIN                                   m_Domain;
     
    std::vector<ScalarParamDesc>                    m_ScalarParam;
    std::vector<TexParamDesc>                       m_TexParam;

public:
    int CreateVertexShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);
    int CreateHullShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);
    int CreateDomainShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);
    int CreateGeometryShader(const std::wstring& _strRelativePath, const std::string& _strFuncName); 
    int CreatePixelShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);

    bool Load(const std::wstring& _relativePath, bool _isFullPath) override;
    bool Save() override;

    SHADER_DOMAIN GetDomain() { return m_Domain; }

    void AddScalarParam(SCALAR_PARAM _type, const string& _desc) { m_ScalarParam.push_back({ _type, _desc }); }
    void AddTexParam(TEX_PARAM _type, const string& _desc) { m_TexParam.push_back({ _type, _desc }); }

    const vector<ScalarParamDesc>& GetScalarParam() { return m_ScalarParam; }
    const vector<TexParamDesc>& GetTexParam() { return m_TexParam; }

private:
    int UpdateData() override;


public:
    GraphicsShader();
    ~GraphicsShader();

    friend class Material;
    friend class ResourceManager;
};

