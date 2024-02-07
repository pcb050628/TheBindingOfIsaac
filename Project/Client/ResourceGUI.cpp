#include "pch.h"
#include "ResourceGUI.h"

ResourceGUI::ResourceGUI(const std::string& _strName, const std::string& _strID, RESOURCE_TYPE _type) : GUI(_strName, _strID)
	, m_Type(_type)
{
	Deactivate();
}

ResourceGUI::~ResourceGUI()
{
}
