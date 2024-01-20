#pragma once
#include "GUI.h"

#include "ImGuiManager.h"

class ListGUI :
    public GUI
{
private:
    std::vector<std::string>    m_strData;
    std::string                 m_LatestClickData;

    CALL_BACK_1                 m_CallBackFunc;
    
    GUI*                        m_GUI;
    Delegate_1                  m_MemberFunc;

public:
    void RenderUpdate() override;

    void AddString(const std::string& _str) { m_strData.push_back(_str); }
    void AddString(const std::vector<std::string>& _str) 
    { 
        m_strData.insert(m_strData.end(), _str.begin(), _str.end());
    }

    void SetCallBack(CALL_BACK_1 _callBack) { m_CallBackFunc = _callBack; }
    void SetDelegate(GUI* _inst, Delegate_1 _func) { m_GUI = _inst; m_MemberFunc = _func; }

    std::string GetLatestClickData() { return m_LatestClickData; }

    virtual void Deactivate() override;

public:
    ListGUI();
    ~ListGUI();
};

