#pragma once
#include "Room.h"
class EditRoom :
    public Room
{
public:
    virtual bool Load(const std::wstring& _strFilePath) override;
    virtual bool Save() override;

    virtual void Update();

    // 15 x 9 tile / include walls
    // tile is for make room easier

public:
    EditRoom();
    virtual ~EditRoom() override;
};

