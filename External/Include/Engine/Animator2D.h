#pragma once
#include "Component.h"

class Anim;
class Texture;

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

    void CreateAnim(const wstring& _strKey, Texture* _altas, Vec2 _leftTop
        , Vec2 _sliceSize, Vec2 _offsetSize, Vec2 _background, int _frmCount, float _FPS);

    void AddAnim(Anim* _anim);
    void RemoveAnim(const std::wstring& _strKey);
    Anim* GetCurAnim() { return m_CurAnim; }
    void GetAllAnim(std::vector<std::string>& _Out);

    void SetCurAnim(const std::wstring& _key);

    void Play(const std::wstring& _key, bool _repeat = false);

    void Play();
    void Pause(bool _reset);
    void SetRepeat(bool _repeat);

    virtual void SaveToFile(FILE* _file) override;
    virtual void LoadFromFile(FILE* _file) override;

    CLONE(Animator2D)
public:
    //void CreateAnim(Texture* _atlas, );

public:
    Animator2D();
    Animator2D(const Animator2D& _origin);
    ~Animator2D();
};

