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

    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_UA;
    Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    UINT                                                m_ElementSize;
    UINT                                                m_ElementCount;

    SB_TYPE                                             m_SBType;
    bool                                                m_bSysMemMove;

    UINT                                                m_RecentNum_SRV;
    UINT                                                m_RecentNum_UAV;

public:
    int Create(UINT _elementSize, UINT _elementCount, SB_TYPE _type, bool _bSysMemMove = false, void* _pSysMem = nullptr);

    void UpdateData(int _regiNum);
    int UpdateData_CS_SRV(int _regiNum);
    int UpdateData_CS_UAV(int _regiNum);

    void Clear_CS_SRV();
    void Clear_CS_UAV();

    void SetData(void* _data, UINT _elementCount = 0);
    void GetData(void* _destData, UINT _elementCount = 0);

    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

public:
    StructuredBuffer();
    ~StructuredBuffer();

};

