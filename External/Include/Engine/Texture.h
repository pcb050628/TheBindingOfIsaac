#pragma once
#include "Resource.h"

class Texture :
    public Resource
{
private:
    DirectX::ScratchImage                               m_Image;  // 텍스쳐 로딩 및 시스템메모리 관리
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_Tex2D;  // 텍스쳐 데이터를 GPU 메모리에 관리
    D3D11_TEXTURE2D_DESC                                m_Desc;   // 텍스쳐 생성 정보

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_RSView; // 렌더타겟 용도
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_DSView; // 뎊스 스텐실 용도
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRView; // 쉐이더에서 사용하는 용도(텍스쳐 레지스터(t) 바인딩)
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_UAView; // GPGPU(General Purpose GPU) - ComputeShader, 읽기 쓰기 동시가능, (Unordered Register(u) 에 바인딩 가능)

private:
    // 파일에서 이미지를 가져옴, Texture 클래스를 불러오는게 아님
    bool Load(const std::wstring& filePath) override;

public:
    void UpdateData(int _regiNum);

    static void Clear(int _regiNum);

    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

public:
    Texture();
    ~Texture();

    friend class Material;
};

