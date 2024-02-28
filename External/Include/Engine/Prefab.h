#pragma once
#include "Resource.h"

class GameObject;

typedef void(*SAVE_TYPE)(GameObject*, FILE*);
typedef GameObject* (*LOAD_TYPE)(FILE*);

class Prefab :
    public Resource
{
public:
    static SAVE_TYPE m_SaveFunc;
    static LOAD_TYPE m_LoadFunc;

private:
    GameObject* m_ProtoObj;

public:
    virtual bool Load(const std::wstring& _FileName, bool _isFullPath) override;
    virtual bool Save() override;

    CLONE_DISABLE(Prefab)
public:
    Prefab();
    ~Prefab();
};

