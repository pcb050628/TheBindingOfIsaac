#pragma once

class GUI;

typedef void(GUI::* Delegate_0)();
typedef void(GUI::* Delegate_1)(DWORD_PTR);
typedef void(GUI::* Delegate_2)(DWORD_PTR, DWORD_PTR);

typedef void(*CALL_BACK_0)(void);
typedef void(*CALL_BACK_1)(DWORD_PTR);
typedef void(*CALL_BACK_2)(DWORD_PTR, DWORD_PTR);

class ImGuiManager
{
	SINGLETON(ImGuiManager)
private:
	std::map<std::string, GUI*> m_mapGUI;

public:
	void Init(HWND _hMainWnd, Microsoft::WRL::ComPtr<ID3D11Device> _Device, Microsoft::WRL::ComPtr<ID3D11DeviceContext> _Context);
	void Progress();

	GUI* FindGUI(const std::string& _strKey)
	{
		std::map<std::string, GUI*>::iterator iter = m_mapGUI.find(_strKey);
		if (iter == m_mapGUI.end())
			return nullptr;

		return iter->second;
	}

private:
	void Update();
	void Render();

	void CrateGUI();
};

