#pragma once
#include "Shader.h"

class ComputeShader :
    public Shader
{
    Microsoft::WRL::ComPtr<ID3DBlob>            m_CSBlob;
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

    // 그룹 개수
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;

    // 스레드 개수
protected:
    const UINT                  m_ThreadX;
    const UINT                  m_ThreadY;
    const UINT                  m_ThreadZ;

    tMtrlData                   m_Const;

public:
    int Create(const wstring& _strRelativePath, const string& _strFuncName);

public:
    void Execute();

protected:
    UINT GetGroupX() { return m_GroupX; }
    UINT GetGroupY() { return m_GroupY; }
    UINT GetGroupZ() { return m_GroupZ; }

    void SetGroupX(UINT _Group) { m_GroupX = _Group; }
    void SetGroupY(UINT _Group) { m_GroupY = _Group; }
    void SetGroupZ(UINT _Group) { m_GroupZ = _Group; }

private:
    virtual int UpdateData() = 0;
    virtual void Clear() = 0;

public:
    ComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ);
    ~ComputeShader();
};

