#pragma once
#include "GUI.h"

#include <Engine\Resource.h>

class ResourceGUI;
class ContentGUI :
    public GUI
{
private:
    ResourceGUI* m_ResourceGUI[(UINT)RESOURCE_TYPE::END];

public:
    void SetResource(Resource* _resource);

public:
    void RenderUpdate() override;

public:
    ContentGUI();
    ~ContentGUI();
};

