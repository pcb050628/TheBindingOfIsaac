#pragma once
#include "RenderComponent.h"

class MeshRenderer :
    public RenderComponent
{
private:
    void UpdateData() override;
    void Render() override;

public:
    MeshRenderer();
    ~MeshRenderer();
};

