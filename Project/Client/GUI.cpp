#include "pch.h"
#include "GUI.h"


GUI::GUI(const std::string& _strName, const std::string& _strID)
	: m_strName(_strName)
	, m_strID(_strID)
	, m_bModal(false)
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
	if (!m_bActive)
		return;

	bool Active = m_bActive;

	if (nullptr == m_Parent)
	{
		if (!m_bModal)
		{
			ImGui::Begin(std::string(m_strName + m_strID).c_str(), &Active);

			if (Active != m_bActive)
			{
				m_bActive = Active;

				if (m_bActive)
					Activate();
				else
					Deactivate();
			}

			RenderUpdate();

			for (int i = 0; i < m_Child.size(); i++)
			{
				m_Child[i]->Render();
			}

			ImGui::End();
		}
		else
		{

			ImGui::OpenPopup(std::string(m_strName + m_strID).c_str());
			if (ImGui::BeginPopupModal(std::string(m_strName + m_strID).c_str(), &Active))
			{
				RenderUpdate();

				for (size_t i = 0; i < m_Child.size(); ++i)
				{
					m_Child[i]->Render();
				}

				ImGui::EndPopup();
			}

			else
			{
				// 활성화, 비활성화 전환이 발생한 경우에는 Activate or Deactivate 를 호출시킨다.
				if (Active != m_bActive)
				{
					m_bActive = Active;

					if (m_bActive)
						Activate();
					else
						Deactivate();
				}
			}
		}
	}
	else
	{
		ImGui::BeginChild(std::string(m_strName + m_strID).c_str(), m_Size);

		RenderUpdate();

		for (int i = 0; i < m_Child.size(); i++)
		{
			m_Child[i]->Render();
		}

		ImGui::EndChild();
	}
}