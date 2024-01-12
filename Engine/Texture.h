#pragma once
#include "Resource.h"

class Texture :
    public Resource
{
private:
    DirectX::ScratchImage                               m_Image;  // �ؽ��� �ε� �� �ý��۸޸� ����
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_Tex2D;  // �ؽ��� �����͸� GPU �޸𸮿� ����
    D3D11_TEXTURE2D_DESC                                m_Desc;   // �ؽ��� ���� ����

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_RTView; // ����Ÿ�� �뵵
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_DSView; // �X�� ���ٽ� �뵵
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRView; // ���̴����� ����ϴ� �뵵(�ؽ��� ��������(t) ���ε�)
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_UAView; // GPGPU(General Purpose GPU) - ComputeShader, �б� ���� ���ð���, (Unordered Register(u) �� ���ε� ����)

private:
    bool Load(const std::wstring& filePath) override;
    int Create(UINT _Width, UINT _Height
        , DXGI_FORMAT _Format, UINT _BindFlag
        , D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);

    int Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex2D);

public:
    void UpdateData(int _regiNum);

    static void Clear(int _regiNum);

    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

    Microsoft::WRL::ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      GetRTV() { return m_RTView; }
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      GetDSV() { return m_DSView; }
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRView; }
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAView; }

public:
    Texture();
    ~Texture();

    friend class Material;
    friend class ResourceManager;
};

