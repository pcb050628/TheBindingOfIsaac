#pragma once

#include "imgui.h"

class GUI
{
private:
	std::string			m_strName;
	const std::string	m_strID;

	bool				m_bActive;

	GUI*				m_Parent;
	std::vector<GUI*>	m_Child;

public:
	void SetName(const std::string& _name) { m_strName = _name; }
	const std::string& GetName() { return m_strName; }

	void AddChild(GUI* _ui)
	{
		m_Child.push_back(_ui);
	}
	GUI* GetParent() { return m_Parent; }

	void Activate() { m_bActive = true; }
	void Deactivate() { m_bActive = false; }
	bool IsActive() { return m_bActive; }

public:
	virtual void Update();
	virtual void Render();
	virtual void RenderUpdate() = 0;

public:
	GUI(const std::string& _strName, const std::string& _strID);
	~GUI();
};

