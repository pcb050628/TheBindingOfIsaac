#pragma once
#include "Resource.h"
class Shader :
    public Resource
{
protected:
    Microsoft::WRL::ComPtr<ID3DBlob> m_pErrBlob;

public:
    virtual void UpdateData();

public:
    Shader(ResourceType _type);
    ~Shader();
};

