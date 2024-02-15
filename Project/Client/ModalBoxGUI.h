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
	void SetCaption(const std::string& _caption) { m_Caption = _caption; }
	void SetContext(const std::string& _context) { m_Context = _context; }
	void Set(const std::string& _caption, const std::string& _context) { m_Caption = _caption; m_Context = _context; }

public:
	ModalBoxGUI();
	~ModalBoxGUI();
};

