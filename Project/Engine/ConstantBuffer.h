#pragma once
#include "Entity.h"
class ConstantBuffer :
    public Entity
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC                       m_Desc;

    UINT                                    m_ElementSize;
    UINT                                    m_ElementCount;

    const CB_TYPE                           m_Type;

public:
    int Create(UINT _Size, UINT _Count);
    void SetData(void* _Src, UINT _ElementCount = 0);
    void UpdateData();
    void UpdateData_CS();

public:
    Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer() { return m_CB; }

public:
    ConstantBuffer(CB_TYPE _type);
    ~ConstantBuffer();
};

