#pragma once
#include "Entity.h"

enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class StructuredBuffer :
    public Entity
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_SB;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_SRV;

    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_SB_Read;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_SB_Write;

    UINT                                                m_ElementSize;
    UINT                                                m_ElementCount;

    SB_TYPE                                             m_SBType;
    bool                                                m_bSysMemMove;

public:
    int Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type, bool _bSysMemMove = false);

    void SetData(void* _data, UINT _elementCount = 0);
    void GetData(void* _destData, UINT _elementCount = 0);

    void UpdateData(int _regiNum);

public:
    StructuredBuffer();
    ~StructuredBuffer();

};

