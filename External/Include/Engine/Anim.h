#pragma once
#include "Resource.h"

struct Frame
{
    Vec2 vLeftTop;
    Vec2 vSliceSize;
    Vec2 vOffset;
    Vec2 vBackground;
};

class Texture;

class Anim :
    public Resource
{
private:
    Texture*            m_Atlas;

    std::vector<Frame>  m_Frames;
    int                 m_CurFrameIdx;
    float               m_fDuration;
    float               m_fAccTime;

    bool                m_bIsPlaying;
    bool                m_bIsRepeat;

public:
    void UpdateData();
    void LateUpdate();

    void Play() { m_bIsPlaying = true; }
    void Pause() { m_bIsPlaying = false; }
    void Reset() { m_CurFrameIdx = 0; }

    bool IsPlaying() { return m_bIsPlaying; }

    void SetRepeat(bool _value) { m_bIsRepeat = _value; }
    bool IsRepeat() { return m_bIsRepeat; }

public:
    bool Load(const std::wstring& _FileName) override;
    bool Save() override;

    void Create(Texture* _atlas, Vec2 _leftTop, Vec2 _sliceSize, Vec2 _offset, Vec2 _background, int _frmCount, int _FPS);

    void CreateNewFrame();
    void RemoveCurFrame();

    Texture* GetAtlas() { return m_Atlas; }
    Frame& GetCurFrame() { return m_Frames[m_CurFrameIdx]; }
    int GetCurFrameIdx() { return m_CurFrameIdx; }
    int GetMaxFrameIdx() { return m_Frames.size(); }
    float GetDuration() { return m_fDuration; }

    void SetAtlas(Texture* _atlas) { m_Atlas = _atlas; }
    void SetCurFrameIdx(int _idx) 
    {
        if (_idx < m_Frames.size()) m_CurFrameIdx = _idx; 
    }
    void SetDuration(float _duration) { m_fDuration = _duration; }


public:
    Anim();
    ~Anim();
};

