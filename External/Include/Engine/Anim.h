#pragma once
#include "Resource.h"

struct Frame
{
    Vec2 vLeftTop;
    Vec2 vSliceSize;
    Vec2 vOffset;
    Vec2 vBackground;
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

    void Create(Texture* _atlas, Vec2 _leftTop, Vec2 _sliceSize, Vec2 _offset, Vec2 _background, int _frmCount, int _FPS);

public:
    Anim();
    ~Anim();
};

