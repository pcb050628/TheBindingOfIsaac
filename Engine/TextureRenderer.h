#pragma once
#include "Component.h"
#include "Texture.h"
#include "math.h"

class TextureRenderer :
    public Component
{
    typedef Component Super;
private:
    Texture*    m_Texture;
    Vec2        m_Offset;

public:
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;

public:
    void SetTexture(Texture* _texture) { m_Texture = _texture; }

    void SetOffset(Vec2 _offset) { m_Offset = _offset; }
    Vec2 GetOffset() { return m_Offset; }

public:
    TextureRenderer();
    ~TextureRenderer() override;
};

