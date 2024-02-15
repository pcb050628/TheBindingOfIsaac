#include "pch.h"
#include "ModalBoxGUI.h"

ModalBoxGUI::ModalBoxGUI() : GUI("ModalBox", "##ModalBoxGUI")
{
	Deactivate();
	SetModal(true);
}

ModalBoxGUI::~ModalBoxGUI()
{
}

void ModalBoxGUI::RenderUpdate()
{
	float windowWidth = ImGui::GetWindowSize().x;
	float captionWidth = ImGui::CalcTextSize(m_Caption.c_str()).x;
	float contextWidth = ImGui::CalcTextSize(m_Context.c_str()).x;

	float xsize = contextWidth;
	if (xsize < captionWidth)
		xsize = captionWidth;

	ImGui::SetWindowSize(ImVec2(xsize * 1.3f, xsize));

	ImGui::SetCursorPosX((windowWidth - captionWidth) * 0.5f);
	ImGui::Text(m_Caption.c_str());

	ImGui::SetCursorPosX((windowWidth - contextWidth) * 0.5f);
	ImGui::Text(m_Context.c_str());

	if (ImGui::Button("OK##ModalBoxGUIOKButton"))
	{
		Deactivate();
	}
}
