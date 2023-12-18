#pragma once
#include "RenderComponent.h"

class MeshRenderer :
    public RenderComponent
{
private:

public:
    void UpdateData() override;
    void Render() override;

public:
    MeshRenderer();
    ~MeshRenderer();
};

