#pragma once
#include "Asset.h"

class Texture :
    public Asset
{
private:
    RECT m_ImageSection;

private:
    bool Load(std::wstring _path) override;
    bool Save() override;
    bool Create(std::wstring _resourcePath, std::wstring _resourceName) override;

public:
    void SetImageSection(RECT _iSection) { m_ImageSection = _iSection; }
    void SetImageSection(int left, int top, int right, int bottom) { 
        m_ImageSection.left = (LONG)left;
        m_ImageSection.top = (LONG)top;
        m_ImageSection.right = (LONG)right;
        m_ImageSection.bottom = (LONG)bottom;
    }
    void SetImageSection(float left, float top, float right, float bottom) {
        m_ImageSection.left = (LONG)left;
        m_ImageSection.top = (LONG)top;
        m_ImageSection.right = (LONG)right;
        m_ImageSection.bottom = (LONG)bottom;
    }
    void Render(Vec2 _pos);

public:
    Texture();
    ~Texture() override;
};

