#include "pch.h"
#include "ContentGUI.h"

#include "ResourceGUI.h"

#include "TextureGUI.h"

ContentGUI::ContentGUI() : GUI("Content", "##ContentGUI")
{
	m_ResourceGUI[(UINT)RESOURCE_TYPE::TEXTURE] = new TextureGUI;
	AddChild(m_ResourceGUI[(UINT)RESOURCE_TYPE::TEXTURE]);
}

ContentGUI::~ContentGUI()
{
}

void ContentGUI::SetResource(Resource* _resource)
{
	for (ResourceGUI* gui : m_ResourceGUI)
	{
		if(gui)
			gui->Deactivate();
	}

	m_ResourceGUI[(UINT)_resource->GetResourceType()]->Activate();
	m_ResourceGUI[(UINT)_resource->GetResourceType()]->SetTarget(_resource);
}

void ContentGUI::RenderUpdate()
{

}
