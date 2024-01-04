#pragma once
#include "Resource.h"
class Shader :
    public Resource
{
protected:
    Microsoft::WRL::ComPtr<ID3DBlob> m_pErrBlob;

public:
    virtual void UpdateData() = 0;

public:
    Shader(RESOURCE_TYPE _type);
    ~Shader();
};

