#pragma once
#include "Asset.h"



struct Frame
{
    Vec2 vLeftTop;  // 아틀라스 내에서 잘라낼 좌상단 위치
    Vec2 vCutSize;  // 잘라낼 크기
    Vec2 vOffset;   // 오브젝트 중심에서 이동 값
    float Duration; // 해당 프레임 노출 시간

    Frame()
        : vLeftTop()
        , vCutSize()
        , vOffset()
        , Duration(0.0f)
    {}
};

class ShaderTextureResource;
class Anim :
    public Asset
{
    typedef Asset Super;
private:
    std::vector<Frame>      m_Frames;
    ShaderTextureResource*  m_Atlas;

    bool                    m_bIsPlaying;

    int                     m_iCurFrame;
    float                   m_AccTime;
    bool                    m_bFinish;

private:
    virtual bool Load(std::wstring _FilePath) override;
    virtual bool Save() override;
    virtual bool Create(std::wstring _resourcePath, std::wstring _resourceName) override;

public:
    virtual void LateUpdate();
    virtual void Render(Vec2 _pos);

public:
    void Play() { m_bIsPlaying = true; }
    void Stop() { m_bIsPlaying = false; }
    bool GetIsPlay() { return m_bIsPlaying; }

public:
    Anim();
    ~Anim() override;

};

