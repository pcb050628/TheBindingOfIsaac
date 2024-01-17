#pragma once

class GUI;
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

