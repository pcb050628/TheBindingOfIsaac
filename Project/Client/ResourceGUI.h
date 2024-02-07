#pragma once
#include "GUI.h"

#include <Engine\Resource.h>

class ResourceGUI :
    public GUI
{
private:
    Resource* m_Target;
    RESOURCE_TYPE m_Type;

public:
    virtual void RenderUpdate() = 0;
    virtual void SetTarget(Resource* _target) { m_Target = _target; }

protected:
    Resource* GetResouce() { return m_Target; }

public:
    ResourceGUI(const std::string& _strName, const std::string& _strID, RESOURCE_TYPE _type);
    ~ResourceGUI();
};

