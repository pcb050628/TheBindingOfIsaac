#pragma once
#include "Resource.h"

class Mesh :
    public Resource
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer>    m_pVtxBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>    m_pIdxBuffer;

    D3D11_BUFFER_DESC                       m_VBDesc;
    D3D11_BUFFER_DESC                       m_IBDesc;

    UINT                                    m_iVtxCount;
    UINT                                    m_iIdxCount;

    void*                                   m_pVtxSysMem;
    void*                                   m_pIdxSysMem;

public:
    virtual bool Load(std::wstring _FilePath) override { return false; }
    int Create(void* _vtx, UINT _vtxCount, void* _idx, UINT _idxCount);

public:
    void UpdateData();
    void Render();

public:
    Mesh();
    ~Mesh();
};

