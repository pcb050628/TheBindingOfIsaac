#pragma once
#include "Resource.h"

struct Frame
{
    Vec2 vLeftTop;
    Vec2 vSliceSize;
    Vec2 vOffset;
    float fDuration;
};

class Texture;

class Anim :
    public Resource
{
private:
    Texture*            m_Atlas;

    std::vector<Frame>  m_Frames;
    int                 m_CurFrameIdx;
    float               m_fAccTime;

    bool                m_bIsPlaying;
    bool                m_bIsRepeat;

public:
    void UpdateData();
    void LateUpdate();

    void Play() { m_bIsPlaying = true; }
    void Pause() { m_bIsPlaying = false; }

    void SetRepeat(bool _value) { m_bIsRepeat = _value; }

public:
    bool Load(const std::wstring& _strFilePath) override;
    bool Save() override;

public:
    Anim();
    ~Anim();
};

