#pragma once
#include "Resource.h"

class Texture :
    public Resource
{
private:
    DirectX::ScratchImage                               m_Image;  // 텍스쳐 로딩 및 시스템메모리 관리
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_Tex2D;  // 텍스쳐 데이터를 GPU 메모리에 관리
    D3D11_TEXTURE2D_DESC                                m_Desc;   // 텍스쳐 생성 정보

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_RTView; // 렌더타겟 용도
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_DSView; // 뎊스 스텐실 용도
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRView; // 쉐이더에서 사용하는 용도(텍스쳐 레지스터(t) 바인딩)
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_UAView; // GPGPU(General Purpose GPU) - ComputeShader, 읽기 쓰기 동시가능, (Unordered Register(u) 에 바인딩 가능)

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

