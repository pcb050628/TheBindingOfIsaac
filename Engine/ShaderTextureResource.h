#pragma once
#include "Resource.h"
#include "wrl.h"
#include "directxtk/SpriteBatch.h"

class ShaderTextureResource :
    public Resource
{
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;

public:
    virtual bool Load(std::wstring _path) override;

public:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureView() { return m_TextureView; }

public:
    ShaderTextureResource();
    ~ShaderTextureResource() override;
};

