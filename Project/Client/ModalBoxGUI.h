#pragma once
#include "GUI.h"

class ModalBoxGUI :
    public GUI
{
private:
	std::string m_Caption;
	std::string m_Context;

public:
	virtual void RenderUpdate() override;

public:
	ModalBoxGUI();
	~ModalBoxGUI();
};

