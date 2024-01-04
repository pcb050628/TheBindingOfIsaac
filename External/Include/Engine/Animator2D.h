#pragma once
#include "Component.h"

class Anim;

class Animator2D :
    public Component
{
private:
    std::map<std::wstring, Anim*>   m_Anims;
    Anim*                           m_CurAnim;

public:
    static void Clear();

    void UpdateData() override;
    void LateUpdate() override;

    void AddAnim(Anim* _anim);

    void Play(const std::wstring& _key, bool _repeat = false);

    void Play();
    void Pause();
    void SetRepeat(bool _repeat);

public:
    //void CreateAnim(Texture* _atlas, );

public:
    Animator2D();
    ~Animator2D();
};

