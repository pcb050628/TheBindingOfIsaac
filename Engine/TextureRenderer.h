#pragma once
#include "Component.h"
#include "Texture.h"

class TextureRenderer :
    public Component
{
    typedef Component Super;
private:
    Texture* m_Texture;

public:
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;

public:
    void SetTexture(Texture* _texture) { m_Texture = _texture; }

public:
    TextureRenderer();
    ~TextureRenderer() override;
};

