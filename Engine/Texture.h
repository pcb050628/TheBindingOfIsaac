#pragma once
#include "Resource.h"
#include "wrl.h"
#include "directxtk/SpriteBatch.h"

class Texture :
    public Resource
{
    typedef Resource Super;
private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_TextureView;
    RECT m_ImageSection;

public:
    bool Load(std::wstring _path) override;
    Texture* Create(std::wstring _ResourcePath);
    bool Save() override;

public:
    void SetImageSection(RECT _iSection) { m_ImageSection = _iSection; }
    void SetImageSection(int left, int top, int right, int bottom) { 
        m_ImageSection.left = left;
        m_ImageSection.top = top;
        m_ImageSection.right = right;
        m_ImageSection.bottom = bottom; 
    }
    void Render(struct Vec2 _pos);

public:
    Texture();
    ~Texture() override;
};

