#pragma once

#include "imgui.h"

//typedef std::function<void()> CALL_BACK_0;
//typedef std::function<void(DWORD_PTR)> CALL_BACK_1;
//typedef std::function<void(DWORD_PTR, DWORD_PTR)> CALL_BACK_2;

typedef void(*CALL_BACK_0)(void);
typedef void(*CALL_BACK_1)(DWORD_PTR);
typedef void(*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

class GUI
{
private:
	std::string			m_strName;
	const std::string	m_strID;
	ImVec2				m_Size;
	bool				m_bModal;

	bool				m_bActive;

	GUI*				m_Parent;
	std::vector<GUI*>	m_Child;

public:
	void SetName(const std::string& _name) { m_strName = _name; }
	const std::string& GetName() { return m_strName; }
	const std::string& GetID() { return m_strID; }

	void SetSize(ImVec2 _size) { m_Size = _size; }
	ImVec2 GetSize() { return m_Size; }

	void AddChild(GUI* _ui)
	{
		m_Child.push_back(_ui);
		_ui->m_Parent = this;
	}
	GUI* GetParent() { return m_Parent; }

	void SetModal(bool _bValue) { m_bModal = _bValue; }
	bool IsModal() { return m_bModal; }

	virtual void Activate() { m_bActive = true; }
	virtual void Deactivate() { m_bActive = false; }
	bool IsActive() { return m_bActive; }

public:
	virtual void Update();
	virtual void Render();
	virtual void RenderUpdate() = 0;

public:
	GUI(const std::string& _strName, const std::string& _strID);
	virtual ~GUI();
};

