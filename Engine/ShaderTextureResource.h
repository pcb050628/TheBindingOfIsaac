#pragma once
#include "Resource.h"
#include "wrl.h"
#include "directxtk/SpriteBatch.h"

class ShaderTextureResource :
    public Resource
{
    typedef Resource Super;
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;

public:
    bool Load(std::wstring _path) override;

public:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTextureView() { return m_TextureView; }

public:
    ShaderTextureResource();
    virtual ~ShaderTextureResource() override;
};

