#pragma once
#include "Shader.h"
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

    std::wstring                                    m_VSPath;
    std::string                                     m_VSFuncName;

    std::wstring                                    m_HSPath;
    std::string                                     m_HSFuncName;

    std::wstring                                    m_DSPath;
    std::string                                     m_DSFuncName;

    std::wstring                                    m_GSPath;
    std::string                                     m_GSFuncName;

    std::wstring                                    m_PSPath;
    std::string                                     m_PSFuncName;

    SHADER_DOMAIN                                   m_Domain;

public:
    int CreateVertexShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);
    int CreateHullShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);
    int CreateDomainShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);
    int CreateGeometryShader(const std::wstring& _strRelativePath, const std::string& _strFuncName); 
    int CreatePixelShader(const std::wstring& _strRelativePath, const std::string& _strFuncName);

    bool Load(const std::wstring& _relativePath) override;
    bool Save() override;

    SHADER_DOMAIN GetDomain() { return m_Domain; }

private:
    int UpdateData() override;


public:
    GraphicsShader();
    ~GraphicsShader();

    friend class Material;
};

