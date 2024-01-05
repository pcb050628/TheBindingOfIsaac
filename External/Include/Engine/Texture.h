#pragma once
#include "Resource.h"

class Texture :
    public Resource
{
private:
    DirectX::ScratchImage                               m_Image;  // �ؽ��� �ε� �� �ý��۸޸� ����
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_Tex2D;  // �ؽ��� �����͸� GPU �޸𸮿� ����
    D3D11_TEXTURE2D_DESC                                m_Desc;   // �ؽ��� ���� ����

    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      m_RSView; // ����Ÿ�� �뵵
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      m_DSView; // �X�� ���ٽ� �뵵
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRView; // ���̴����� ����ϴ� �뵵(�ؽ��� ��������(t) ���ε�)
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_UAView; // GPGPU(General Purpose GPU) - ComputeShader, �б� ���� ���ð���, (Unordered Register(u) �� ���ε� ����)

private:
    // ���Ͽ��� �̹����� ������, Texture Ŭ������ �ҷ����°� �ƴ�
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

