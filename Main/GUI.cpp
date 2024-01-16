#include "pch.h"
#include "GUI.h"


GUI::GUI(const std::string& _strName, const std::string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bActive(true)
	, m_Parent(nullptr)
	, m_Child{}
{
}

GUI::~GUI()
{
	for (int i = 0; i < m_Child.size(); i++)
	{
		delete m_Child[i];
	}

	m_Child.clear();
}

void GUI::Update()
{
}

void GUI::Render()
{
	if (m_bActive)
	{
		if (nullptr == m_Parent)
		{
			ImGui::Begin(std::string(m_strName + m_strID).c_str());

			RenderUpdate();

			for (int i = 0; i < m_Child.size(); i++)
			{
				m_Child[i]->Render();
			}

			ImGui::End();
		}
		else
		{
			ImGui::BeginChild(std::string(m_strName + m_strID).c_str());

			RenderUpdate();

			for (int i = 0; i < m_Child.size(); i++)
			{
				m_Child[i]->Render();
			}

			ImGui::EndChild();
		}
	}
}