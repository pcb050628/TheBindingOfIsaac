#pragma once
#include "Resource.h"
#include "math.h"

struct Frame
{
    Vec2 vLeftTop;  // 아틀라스 내에서 잘라낼 좌상단 위치
    Vec2 vCutSize;  // 잘라낼 크기
    Vec2 vOffset;   // 오브젝트 중심에서 이동 값
    float Duration; // 해당 프레임 노출 시간
};

class Texture;
class Anim :
    public Resource
{
    typedef Resource Super;
private:
    std::vector<Frame> m_Frames;
    Texture* m_Atlas;
    int m_CurFrame;

    float m_AccTime;

public:
    virtual bool Load(std::wstring _FilePath) override;
    Anim* Create(std::wstring _ResourcePath);
    virtual bool Save() override;

public:
    virtual void LateUpdate();
    virtual void Render(Vec2 _pos);

public:
    Anim();
    ~Anim() override;

};

