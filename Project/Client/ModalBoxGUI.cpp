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
	float textWidth = ImGui::CalcTextSize(m_Caption.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(m_Caption.c_str());

	ImGui::Button("OK##ModalBoxGUIOKButton");
}
