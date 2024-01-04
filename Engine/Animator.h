#pragma once
#include "Component.h"
#include "Anim.h"

class Animator :
    public Component
{
    typedef Component Super;
private:
    std::map<AssetID, Anim*>    m_Anims;
    Anim*                       m_CurAnim;

public:
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;

public:
    void AddAnim(Anim* _anim) { m_Anims.insert(std::make_pair(_anim->GetAssetID(), _anim)); }

    bool Play()
    {
        if (m_CurAnim != nullptr)
        {
            m_CurAnim->Play();
            return true;
        }
        return false;
    }
    bool Play(AssetID _id)
    {
        auto iter = m_Anims.find(_id);
        if (iter != m_Anims.end())
        {
            m_CurAnim = iter->second;
            m_CurAnim->Play();
            return true;
        }
        return false;
    }
    bool StopPlay()
    {
        if (m_CurAnim != nullptr)
        {
            m_CurAnim->Stop();
            return true;
        }
        return false;
    }

    bool GetIsPlaying()
    {
        if (m_CurAnim == nullptr)
            return false;
        else
            return m_CurAnim->GetIsPlay();
    }

    AssetID GetCurAnimID() { return m_CurAnim->GetAssetID(); }

public:
    Animator();
    ~Animator() override;
};

